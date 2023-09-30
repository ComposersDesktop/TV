/*
 *	print the syntax tree of a tv program
 */
#include <stdio.h>
#include <stdlib.h>
#include "tv.h"
#include "xtab.h"

static void prexpr(Expr *e, FILE *f, int level);

static void
ind(FILE *f, int level)
{
	int c;

	for(c = 0; c < level; c++)
		fprintf(f, "    ");
}

static void
prbiop(char *op, Expr *e, FILE *f, int level)
{
	ind(f, level);
	fprintf(f, "%s\n", op);
	prexpr(e->left, f, level+1);
	prexpr(e->right, f, level+1);
}

static void
prexpr(Expr *e, FILE *f, int level)
{
	Exprlist *el;

	switch(e->op) {
	case NUMBER:
		ind(f, level);
		fprintf(f, "%f\n", e->value);
		return;

	case UNARY_MINUS:
		ind(f, level);
		fprintf(f, "Unary Minus\n");
		prexpr(e->left, f, level+1);
		return;

	case '+':	prbiop("OP +", e, f, level); return;
	case '-':	prbiop("OP -", e, f, level); return;
	case '*':	prbiop("OP *", e, f, level); return;
	case '/':	prbiop("OP /", e, f, level); return;
	case '%':	prbiop("OP %", e, f, level); return;
	case '^':	prbiop("OP ^", e, f, level); return;
	case '&':	prbiop("OP &", e, f, level); return;
	case '|':	prbiop("OP |", e, f, level); return;
	case '<':	prbiop("OP <", e, f, level); return;
	case '>':	prbiop("OP >", e, f, level); return;
	case EQ:	prbiop("OP ==", e, f, level); return;
	case '!':	prbiop("OP !=", e, f, level); return;
	case LESS_EQ:	prbiop("OP <=", e, f, level); return;
	case GTHAN_EQ:	prbiop("OP >=", e, f, level); return;
	case ANDAND:	prbiop("OP &&", e, f, level); return;
	case OROR:	prbiop("OP ||", e, f, level); return;

	case MFUNC:
		ind(f, level);
		fprintf(f, "%s\n", e->mfunc->picture);
		switch(e->mfunc->arity) {
		case 1:
			prexpr(e->left, f, level+1);
			return;
		case 2:
			prexpr(e->left, f, level+1);
			prexpr(e->right, f, level+1);
			return;
		default:
			fprintf(stderr, "TV: internal: bad case in MFUNC\n");
			exit(1);
		}

	case '[':
		ind(f, level);
		fprintf(f, "index\n");
		prexpr(e->left, f, level+1);
		for(el = e->el; el != 0; el = el->next)
			prexpr(el->e, f, level+1);
		return;

	case IDENT:
		ind(f, level);
		fprintf(f, "IDENT %s(%p) offset = %d\n", e->sym->picture, e->sym, e->indx);
		return;

	case TIDENT:
		ind(f, level);
		fprintf(f, "TIDENT %s(%p) offset = %d\n", e->sym->picture, e->sym, e->indx);
		return;

	default:
		fprintf(stderr, "checkoper pr - operation %d not defined yet\n", e->op);
		return;
	}
}

static void
prstat(Statement *s, FILE *f, int level)
{
	Exprlist *el;

	for(; s != 0; s = s->next) {
		switch(s->code) {
		case RULE:
			ind(f, level);
			fprintf(f, "RULE %s\n", s->rule->picture);
			for(el = s->el; el != 0; el = el->next)
				prexpr(el->e, f, level+1);
			break;
		case IF:
			ind(f, level); fprintf(f, "IF\n");
			prexpr(s->cond, f, level+1);
			ind(f, level); fprintf(f, "THEN\n");
			prstat(s->body1, f, level+1);
			break;

		case ELSE:
			ind(f, level); fprintf(f, "IF-ELSE\n");
			prexpr(s->cond, f, level+1);
			ind(f, level); fprintf(f, "THEN\n");
			prstat(s->body1, f, level+1);
			ind(f, level); fprintf(f, "ELSE\n");
			prstat(s->body2, f, level+1);
			break;

		case WHILE:
			ind(f, level); fprintf(f, "WHILE\n");
			prexpr(s->cond, f, level+1);
			ind(f, level); fprintf(f, "DO\n");
			prstat(s->body1, f, level+1);
			break;

		case FOR:
			ind(f, level); fprintf(f, "FOR\n");
			prstat(s->forinit, f, level+1);
			ind(f, level); fprintf(f, "WHILE\n");
			prexpr(s->cond, f, level+1);
			ind(f, level); fprintf(f, "INCR\n");
			prstat(s->body2, f, level+1);
			ind(f, level); fprintf(f, "DO\n");
			prstat(s->body1, f, level+1);
			break;

		case CALL:
			ind(f, level); fprintf(f, "CALL %s\n", s->label->picture);
			for(el = s->el; el != 0; el = el->next)
				prexpr(el->e, f, level+1);
			break;

		case END:
			ind(f, level); fprintf(f, "END PROGRAM\n");
			break;

                case STORSTR:
                  ind(f, level); fprintf(f, "STORSTR\n");
                  break;
                  
                case STOREFILE:
                  ind(f, level); fprintf(f, "STOREFILE\n");
                  break;
                  
                case FILLTABLES:
                  ind(f, level); fprintf(f, "FILLTABLES\n");
                  break;

                case '=':
                  ind(f, level); fprintf(f, "=\n");
                  break;
                  
		default:
			fprintf(stderr, "print - operation %d not defined yet\n", s->code);
			break;//exit(1);
		}
	}
}

static void
prproc(Proc *proc, FILE *f)
{
        Param *p;
        Local *l;
	int i = 0;

	ind(f, 1); fprintf(f, "\nPROCEDURE %s(%d)\n", proc->name->picture, proc->arity);
	for(p = proc->plist; p != 0; p = p->next) {
		ind(f, 2); fprintf(f, "%s(%p) at offset %d\n",
				p->sym->picture, p->sym, i++);
	}
	ind(f, 1); fprintf(f, "LOCALS\n");
	for(l = proc->locals; l != 0; l = l->next) {
		ind(f, 2); fprintf(f, "%s(%p) at offset %d\n",
                                   (l->name? l->name->picture:"no name"), l->name,i++); /* ...but l->name is zero -- JPff */
		if(l->type == value)
			prexpr(l->init, f, 3);
		else if(l->init == 0) {
			ind(f, 3); fprintf(f, "NULL\n");
		} else {
			ind(f, 3); fprintf(f, "%s(%p)\n", l->init->sym->picture, l->init->sym);
		}
	}
	ind(f, 1); fprintf(f, "BODY\n");
	prstat(proc->body, f, 2);
}

void
prprog(Program *prog, FILE *f)
{
	Proc *procs;
	Table *tab;
        Global *gl;
	int i;

	/* create global variables for each table */

	fprintf(f, "TABLES:\n");
	for(tab = prog->tables; tab != 0; tab = tab->next) {
		ind(f, 1); fprintf(f, "%s", tab->name->picture);
		for(i = 0; i < tab->tabdim.dimensions; i++)
			fprintf(f, "[%d(%d)]", tab->tabdim.sizes[i],
				(tab->tabdim.spaces != 0) ? tab->tabdim.spaces[i] : -1000);
		fprintf(f, "\n");
	}

	fprintf(f, "GLOBALS:\n");
	for(gl = prog->globals; gl != 0; gl = gl->next) {
		ind(f, 1); fprintf(f, "%s(%p)\n", gl->name->picture, gl->name);
	}

	fprintf(f, "PROCEDURES:\n");
	for(procs = prog->procs; procs != 0; procs = procs->next)
		prproc(procs, f);
}

