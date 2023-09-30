/*
 *	static checks for tv
 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "tv.h"
#include "xtab.h"

static int
checkrule(Statement *s, Proc *proc, Program *prog)
{
	Local *l, **ll = &proc->locals;

	if(s->rule->init != 0) { /* => requires local storage */

		for(; *ll != 0; ll = &(*ll)->next)
			;
		l = *ll = ALLOC(Local);
		l->name = 0;
		l->type = ruledata;
		l->init = 0;
		l->rule = s->rule;
		l->next = 0;
		s->ruledataoffset = proc->arity + proc->numloc++;
	}
	if(s->elr != 0 && s->elr->e->op == STRING) {
		if(!s->rule->string) {
			fprintf(stderr, "TV: rule %s does not take a string parameter\n",
					s->rule->picture);
			return 1;
		}
	}
	if(s->rule->string
	 &&(s->elr == 0 || s->elr->e->op != STRING)) {
		fprintf(stderr, "TV: rule %s requires a string parameter\n",
				s->rule->picture);
		return 1;
	}
	return s->rule->check(s, proc, prog);
}

int
checkident(Expr *e, Proc *proc, Program *prog)
{
	int i = 0;
	Param *pl;
	Local *ll;
	Global **gl;

	for(pl = proc->plist; pl != 0; pl = pl->next)
		if(pl->sym == e->sym)
			break;
		else
			i++;
	/* is it a local? */
	if(pl == 0)
		for(ll = proc->locals; ll != 0; ll= ll->next) {
			if(ll->name == e->sym)
				break;
			else
				i++;
		}
	if(pl != 0 || ll != 0) {	/* parameter/local found */
		e->indx = i;
		return 0;
	}
	/* it is a global! */
	for(gl = &prog->globals; *gl != 0; gl = &(*gl)->next)
		if((*gl)->name == e->sym)
			break;

	if(*gl == 0) {	/* first reference to this global */
		*gl = ALLOC(Global);
		(*gl)->name = e->sym;
		(*gl)->next = 0;
		(*gl)->value.type = istablesym(e->sym) ? tableptr : value;
	}
	e->indx = -1;
	e->glob = *gl;
	return 0;
}

int
checkexpr(Expr *e, Proc *proc, Program *prog)
{
	Exprlist *el;

	switch(e->op) {
	case NUMBER:
		return 0;

	case UNARY_MINUS:
		return checkexpr(e->left, proc, prog);

	case '+': case '-': case '*': case '/': case '%': case '^':
	case ANDAND: case OROR:
	case '<': case '>':
	case LESS_EQ: case GTHAN_EQ:
		return checkexpr(e->left, proc, prog) +
			checkexpr(e->right, proc, prog);

	case EQ: case '!':
		if(e->left->op == TIDENT) {
			return checkident(e->left, proc, prog) +
				checkident(e->right, proc, prog);
		}
		return checkexpr(e->left, proc, prog) +
			checkexpr(e->right, proc, prog);

	case MFUNC:
		switch(e->mfunc->arity) {
		case 0:
			return 0;
		case 1:
			if(e->mfunc->fptab && e->left->op != TIDENT) {
				fprintf(stderr, "TV: parameter of %s must be table\n",
							e->mfunc->picture);
				return 1;
			}
			return checkexpr(e->left, proc, prog);
		case 2:
			if(e->mfunc->fptab &&
			   e->left->op != TIDENT && e->left->op != SIDENT) {
				fprintf(stderr, "TV: first parameter of %s must be table\n",
							e->mfunc->picture);
				return 1;
			}
			return checkexpr(e->left, proc, prog) + checkexpr(e->right, proc, prog);
		default:
			fprintf(stderr, "TV: internal: bad case in MFUNC\n");
			exit(1);
		}
		break;

	case '[':
		{
			int err = 0;
			checkident(e->left, proc, prog);
			for(el = e->el; el != 0; el = el->next)
				err += checkexpr(el->e, proc, prog);
			return err;
		}
		//break;

	case PLUSPLUS:
	case PLUSPLUS+POST:
	case MINUSMINUS:
	case MINUSMINUS+POST:
		if(e->left->op != IDENT) {
			fprintf(stderr, "TV: operand of ++ or -- must be identifier\n");
			return 1;
		}
		return checkexpr(e->left, proc, prog);

	case IDENT:
	case TIDENT:
	case SIDENT:
		return checkident(e, proc, prog);

	case TRY:
		return checkrule(e->rule, proc, prog);
        case LOOP:
          

	case STRING:
		return 0; //checkstring(e->string, proc, prog);

	case '~':
		return 0; //checkstring(e->string, proc, prog);

	default:
		fprintf(stderr, "checkoper - operation %d not defined yet\n", e->op);
		return 1;
	}
	return 0;
}

int
numparam(Exprlist *el)
{
	int cnt = 0;

	for(; el != 0; el = el->next)
		cnt++;
	return cnt;
}

Expr *
getparam(Exprlist *el, int n)
{
	while(n-- > 0) {
		el = el->next;
		if(el == 0)
			abort();
	}
	return el->e;
}

int
checkparam(Exprlist *el, Proc *proc, Program *prog)
{
	int rc = 0;

	for(; el != 0; el = el->next)
		rc += checkexpr(el->e, proc, prog);
	return rc;
}

static int
checkcall(Statement *s, Proc *proc, Program *prog)
{
	int errs = 0;
	int arity  = 0;		/* arity of the call */
	int cnt;
	Exprlist *el;
	Proc *cproc;
	Param *param;

	/* find number of parameters */
	for(el = s->el; el != 0; el = el->next)
		arity++;

	/* find procedure to be called */
	for(cproc = prog->procs; cproc != 0; cproc = cproc->next)
		if(s->label == cproc->name)
			break;
	if(cproc == 0) {
		fprintf(stderr, "TV: procedure %s calls non-existent procedure %s\n",
					proc->name->picture, s->label->picture);
		return 1;
	}
	s->proc = cproc;

	if(arity != cproc->arity) {
		fprintf(stderr, "TV: procedure %s calls %s with %d parameters (should be %d)\n",
					proc->name->picture, s->label->picture, arity, cproc->arity);
		errs = 1;
	}

	/* check actual parameters */
	for(el = s->el, param = cproc->plist, cnt = 0;
	    el != 0 && param != 0;
	    el = el->next, param = param->next, cnt++) {
		errs += checkexpr(el->e, proc, prog);
		if(istablesym(param->sym)) {
			if(el->e->op != TIDENT) {
				fprintf(stderr, "TV: proc %s calls proc %s with parameter %d which is not a table\n",
						proc->name->picture, s->label->picture, cnt);
				errs++;
			}
		}
	}
	return errs;
}

static int
checkstat(Statement *s, Proc *proc, Program *prog)
{
	int errs = 0;

	for(; s != 0; s = s->next) {
		switch(s->code) {
		case RULE:
			errs += checkrule(s, proc, prog);
			break;
		case IF:
			errs += checkexpr(s->cond, proc, prog);
			errs += checkstat(s->body1, proc, prog);
			break;

		case ELSE:
			errs += checkexpr(s->cond, proc, prog);
			errs += checkstat(s->body1, proc, prog);
			errs += checkstat(s->body2, proc, prog);
			break;

		case SWITCH:
		  {
			Cases *t = (Cases*)s->body1;
			errs += checkexpr(s->cond, proc, prog);
			while (t) {
			  errs += checkstat(t->body, proc, prog);
			  t = t->next;
			}
			errs += checkstat(s->body2, proc, prog);
			break;
		  }

		case WHILE:
			errs += checkexpr(s->cond, proc, prog);
			errs += checkstat(s->body1, proc, prog);
			break;

		case FOR:
			errs += checkstat(s->forinit, proc, prog);
			errs += checkexpr(s->cond, proc, prog);
			errs += checkstat(s->body1, proc, prog);
			errs += checkstat(s->body2, proc, prog);
			break;

		case CALL:
			errs += checkcall(s, proc, prog);
			break;

		case BREAK:
		case CONTINUE:
		case LOOP:
		case RETURN:
			break;

		case '=':
			if(s->cond->left->op == TIDENT) {
				if(s->cond->right->op != TIDENT) {
					fprintf(stderr, "TV: can only assign tables to tables\n");
					errs++;
					break;
				}
				if(s->cond->op != '=') {
					fprintf(stderr, "TV: cannot use assignment operators on tables\n");
					errs++;
					break;
				}

				errs += checkident(s->cond->left, proc, prog);
				errs += checkident(s->cond->right, proc, prog);
				break;
			}

			errs += checkexpr(s->cond->left, proc, prog);
			errs += checkexpr(s->cond->right, proc, prog);
			break;

		case FILLTABLE:
		case FILLTABLEA:
		case FILLTABLES:
			if((s->cond->op != TIDENT) && (s->cond->op != SIDENT)) {
				fprintf(stderr,
					"TV: filltable must be given a table\n");
				errs++;
			} else {
				checkident(s->cond, proc, prog);
				if(s->el != 0)
					errs += checkexpr(s->el->e, proc, prog);
				if (s->string != NULL && s->string->op == '[')
				  errs += checkstring(s->string, proc, prog);
			}
			break;

		case SEND:
			errs += checkexpr(s->cond, proc, prog);
			errs += checkparam(s->el, proc, prog);
			if (s->string != NULL)
				errs += checkstring(s->string, proc, prog);
			{
				SEXP *path = (struct sexpr*)s->body1;
				if (s->body1 != NULL)
					errs += checkstring(path, proc, prog);
			}
			break;

		case STORSTR:
		case STOREFILE:
		case OPSYSCALL:
			if (s->string != NULL)
			  errs += checkstring(s->string, proc, prog);
			break;

		case CLS:
		case SHOWARGS:
		case END:
		case DECDIG:
                case TRY:       /* Where should this go>? -- JPff */
			break;

		case '~':
			//printf("Found string assignment\n");
			{
				SEXP* ss = (SEXP*)s->el;
				//printf("**ss=%p\n", ss);
				if (s->string != NULL)
					errs += checkstring(s->string, proc, prog);
				if (ss == NULL || ss->op != '[') errs++;
				errs += checkexpr(ss->el->e, proc, prog);
			}
			break;

		default:
			fprintf(stderr,
				"pass2 - operation %d not defined yet\n", s->code);
			exit(1);
		}
	}
	return errs;
}

static int
checkproc(Proc *proc, Program *prog)
{
	int errs = 0;
	int i;
	Proc *p;
	struct local *l;

	i = 0;
	for(p = prog->procs; p != 0; p = p->next)
		if(proc->name == p->name)
			i++;
	if(i != 1) {
		fprintf(stderr, "TV: procedure %s defined %d times\n", proc->name->picture, i);
		errs++;
	}

	for(l = proc->locals; l != 0; l = l->next) {
		Local *rest = l->next;	/* puts later locals out of scope! */
		l->next = 0;

		switch(l->type) {
		case value:
			checkexpr(l->init, proc, prog);
			break;
		case tableptr:
			if(l->init != 0) {
				assert(l->init->op == TIDENT);
				checkexpr(l->init, proc, prog);
			}
			break;
		default:
			fprintf(stderr,
				"TV: internal: illegal case in checkproc\n");
			exit(1);
		}

		l->next = rest;		/* put later globals back! */
	}

	errs += checkstat(proc->body, proc, prog);
	return errs;
}

int
pass2(Program *prog)
{
	int errs = 0;
	int i, space;
	Proc *procs;
	Table *tab;
	Global **gl;

	/* create global variables for each table */
	for(tab = prog->tables; tab != 0; tab = tab->next) {
		for(gl = &prog->globals; *gl != 0; gl = &(*gl)->next) {
			if((*gl)->name == tab->name) {
				fprintf(stderr, "TV: table %s defined more than once\n",
					tab->name->picture);
				errs++;
			}
		}
		*gl = ALLOC(Global);
		(*gl)->name = tab->name;
		(*gl)->next = 0;
		(*gl)->value.type = tableglobal;
		(*gl)->value.u.table = tab;

		tab->tabdim.spaces = emalloc(tab->tabdim.dimensions * sizeof(int));
		space = 1;
		for(i = tab->tabdim.dimensions - 1; i >= 0; i--) {
			tab->tabdim.spaces[i] = space;
			space *= tab->tabdim.sizes[i];
		}
	}

	/* check all procedures */
	for(procs = prog->procs; procs != 0; procs = procs->next)
		errs += checkproc(procs, prog);
	return errs;
}
