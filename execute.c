/*
 *	execute a tv program
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#endif
#include "tv.h"
#include "xtab.h"
#include "rules.h"
#include "midi.h"


enum runstate {
	breaking,
	conting,
	looping,
	returning,
	normal
};

int sndoverflows;
static Symbol *brkcont_lab;

/*
 *	Debugging support
 */
int dump_globals = 0;

enum runstate exec(Statement *pc, Cell *params);
enum runstate execproc(Proc *sym, Exprlist *el, Cell *params);

void	mktable(Table *tab);
void	filltable(Table *tab, char *filename,
		 Exprlist *el, Cell *params);
void	filltables(TableS *tab, char *filename,
		 Exprlist *el, Cell *params);

Sfile *sfile = NULL;
int     decdig = 2;

//static	int initexprlist(Exprlist *el);

double
eval(Expr *e, Cell *params)
{
	assert(e != 0);

	switch(e->op) {
	case NUMBER:
		return e->value;
	case '+':
		return eval(e->left, params) + eval(e->right, params);
	case '-':
		return eval(e->left, params) - eval(e->right, params);
	case '*':
		return eval(e->left, params) * eval(e->right, params);
	case '/':
		return eval(e->left, params) / eval(e->right, params);
	case '%':
		return fmod(eval(e->left, params), eval(e->right, params));
	case '^':
		return pow(eval(e->left, params), eval(e->right, params));
	case UNARY_MINUS:
		return -eval(e->left, params);
	case ANDAND:
		return (double)(eval(e->left, params) && eval(e->right, params));
	case OROR:
		return (double)(eval(e->left, params) || eval(e->right, params));
	case '<':
		return (double)(eval(e->left, params) < eval(e->right, params));
	case '>':
		return (double)(eval(e->left, params) > eval(e->right, params));
	case EQ:
		if(e->left->op == TIDENT) {
			return (double)(LGVAR(e->left, params)->u.table ==
					LGVAR(e->right, params)->u.table);
		}
		return (double)(eval(e->left, params) == eval(e->right, params));
	case '!':
		if(e->left->op == TIDENT) {
			return (double)(LGVAR(e->left, params)->u.table !=
					LGVAR(e->right, params)->u.table);
		}
		return (double)(eval(e->left, params) != eval(e->right, params));
	case LESS_EQ:
		return (double)(eval(e->left, params) <= eval(e->right, params));
	case GTHAN_EQ:
		return (double)(eval(e->left, params) >= eval(e->right, params));
	case IDENT:
		return LGVAR(e, params)->u.d;
	case PLUSPLUS:
		return ++LGVAR(e->left, params)->u.d;
	case PLUSPLUS+POST:
		return LGVAR(e->left, params)->u.d++;
	case MINUSMINUS:
		return --LGVAR(e->left, params)->u.d;
	case MINUSMINUS+POST:
		return LGVAR(e->left, params)->u.d--;
	case MFUNC:
		switch(e->mfunc->arity) {
		case 0:
			return e->mfunc->exec();
		case 1:
			if(e->mfunc->fptab)
				return e->mfunc->exec(LGVAR(e->left, params)->u.table);
			return e->mfunc->exec(eval(e->left, params));
		case 2:
			if(e->mfunc->fptab)
				return e->mfunc->exec(LGVAR(e->left, params)->u.table,
							eval(e->right, params));
			return e->mfunc->exec(eval(e->left, params),
						eval(e->right, params));
		}
		/* NOTREACHED */
	case '[':		/* dereference a table */
		return *dereftab(LGVAR(e->left, params)->u.table, e->el, params);
	case TRY:
		return (double)e->rule->rule->exec(
					e->rule,
					params,
					&params[e->rule->ruledataoffset].u.ruledata);
	default:
		fprintf(stderr, "operation %d not defined yet(eval)\n", e->op);
		/*RWD 2004: need retval for all paths */
		return 0.0;
	}
}

char* dereftabs(Table*, Exprlist *, Cell*);

/* char *stab2string(Table *sym, Exprlist *n, Cell *par) */
/* { */
/*     //printf("sym:%p n: %p\n", sym, n); */
/*     // Need to deref the sym [ n ] */
/*     return dereftabs(sym, n, par); */
/* } */

extern struct	program *prog;

// Convert a strigexpr into a direct C string
char *sexp(SEXP *ss, Cell *par)
{
    Table *tt;
    if (ss->op == '[') {
      // **** NEED to convert a symbol to a TableS
      for (tt=prog->tables; tt != NULL; tt = tt->next) {
        //printf("ss->sym = %p against %s\n", ss->sym, tt->name->picture);
        if (strcmp(ss->sym->picture,tt->name->picture)==0) break;
      }
      if (tt==NULL)
        for (tt=prog->globals->value.u.table; tt != NULL; tt = tt->next) {
          //printf("ss->sym = %p against %s\n", ss->sym, tt->name->picture);
          if (strcmp(ss->sym->picture,tt->name->picture)==0) break;
        }
      return dereftabs(tt, ss->el, par);
    }
    else if (ss->op == INT2STRING) {
      int val = (int)(0.5+eval(ss->el->e, par));
      if (ss->string) free(ss->string);
      ss->string = (char*)malloc(12);
      sprintf(ss->string, "%d", val);
      return ss->string;
    }
    else if (ss->op == NUM2STRING) {
      double val = eval(ss->el->e, par);
      if (ss->string) free(ss->string);
      ss->string = (char*)malloc(20);
      sprintf(ss->string, "%f", val);
      return ss->string;
    }
    else if (ss->op == BEGCAT) {
      SEXP* tt = ss->next;
      char *res;
      //printf("concatenation case\n");
      if (ss->string==NULL) ss->string = (char*)calloc(1, MAXSTRING);
      while (tt) {
        res = sexp(tt, par);
        strcat(ss->string, res);
        tt = tt->next;
      }
      return ss->string;
    }
    else return ss->string;
}

void
execute(Program *prog)
{
	Symbol *startsym = lookup("start");
	Proc *startproc;
        Global *gl;
	char *errmsg;

	sndoverflows = 0;

	for(startproc = prog->procs; startproc != 0; startproc = startproc->next)
		if(startproc->name == startsym)
			break;
	if(startproc == 0) {
		fprintf(stderr, "TV: no 'start' procedure to execute!\n");
		tidy_up(0);
	}
	if(startproc->arity != 0) {
		fprintf(stderr, "TV: 'start' should not have parameters\n");
		tidy_up(0);
	}

	for(gl = prog->globals; gl != 0; gl = gl->next) {
		switch(gl->value.type) {
		case tableglobal:
			/* create the table */
                        mktable(gl->value.u.table);
			break;
		case tableptr:
			/* null the table reference */
			gl->value.u.table = 0;
			break;
		case value:
			gl->value.lock = 0;	/* cell UNLOCKED */
			gl->value.u.d = 0.0;
			break;
		default:
			fprintf(stderr, "TV: internal execute:globals\n");
			tidy_up(0);
		}

		if(dump_globals)
			fprintf(stderr, "TV before: global %s\n", gl->name->picture);
	}

#ifdef OLD
	if(initialize(prog)) {
		fprintf(stderr, "TV: can't initialize program\n");
		exit(1);
	}
#endif

	switch(execproc(startproc, 0, 0)) {
	case returning:
		errmsg = "return";
		break;
	case looping:
		errmsg = "loop";
		break;
	case conting:
		errmsg = "continue";
		break;
	case breaking:
		errmsg = "break";
		break;
	default:
		errmsg = 0;
		break;
	}
	if(errmsg) {
		assert(brkcont_lab != 0);
		fprintf(stderr, "%s statement cannot find label %s\n",
			errmsg, brkcont_lab->picture);
	}

	if(sndoverflows > 0)
		fprintf(stderr, "TV: %d written samples were out of range\n",
					sndoverflows);
	if(dump_globals) {
		for(gl = prog->globals; gl != 0; gl = gl->next) {
			fprintf(stderr, "tv after: global %s = %f\n",
					gl->name->picture, gl->value.u.d);
			}
	}
}

void
clearscreen(void)
{
#ifdef _WIN32
	cls();
#else
        printf("\f");
#endif
}

void
close_storefiles(void)
{
    while (sfile) {
        write(sfile->storefd, sfile->store_buf, strlen(sfile->store_buf));
        close(sfile->storefd);
        sfile = sfile->next;
    }
}

void
tidy_up(int flag)
{
	mach_cleanup();
	midi_cleanup(1);
	if(flag)
	    perf_time(1);
	while (sfile) {
	    	write(sfile->storefd, sfile->store_buf, strlen(sfile->store_buf));
                close(sfile->storefd);
                sfile = sfile->next;
        }
	exit(0);
}

enum runstate
execproc(Proc *pp, Exprlist *el, Cell *params)
{
	int i;
	Cell *nparams = NULL;
        Param *parp = pp->plist;
        Local *locp = pp->locals;
	enum runstate rs;

	assert(pp != NULL);
	if(pp->arity + pp->numloc > 0)
		nparams = emalloc((pp->arity + pp->numloc)*sizeof(Cell));

	for(i = 0; i < pp->arity; i++) {
		Expr *e;
		assert(el != NULL);
		assert(parp != NULL);

		e = el->e;
		if(istablesym(parp->sym)) {
			assert(e->op == TIDENT);
			nparams[i].type = tableptr;
			nparams[i].u.table = LGVAR(e, params)->u.table;
		} else {
			nparams[i].type = value;
			nparams[i].lock = 0;
			nparams[i].u.d = eval(el->e, params);
		}
		parp = parp->next;
		el = el->next;
	}

	for(; locp != 0; locp = locp->next) {
		switch(locp->type) {
			/* add extra type here for rule local storage */
		case value:
			nparams[i].type = value;
			nparams[i].lock = 0;
			nparams[i].u.d = eval(locp->init, nparams);
			break;
		case tableptr:
			nparams[i].type = tableptr;
			if(locp->init == 0)
				nparams[i].u.table = 0;
			else
				nparams[i].u.table = LGVAR(locp->init, nparams)->u.table;
			break;
		case ruledata:
			nparams[i].type = ruledata;
			nparams[i].rulefree = locp->rule->init(&nparams[i].u.ruledata);
			break;

		default:
			assert(0);
		}
		i++;
	}

again:
	switch(rs = exec(pp->body, nparams)) {
	case returning:
        case normal:
		break;

	case looping:
		if(brkcont_lab == 0 || brkcont_lab == pp->name)
			goto again;
		break;

	case breaking:
		if(brkcont_lab == 0) {
			fprintf(stderr, "TV: 'break' statement cannot exit procedure %s without label\n",
						pp->name->picture);
			tidy_up(0);
		}
		if(brkcont_lab == pp->name)
			rs = normal;
		break;

	case conting:
		if(brkcont_lab == 0) {
			fprintf(stderr, "TV: 'continue' statement cannot exit procedure %s without label\n",
						pp->name->picture);
			tidy_up(0);
		}
		if(brkcont_lab == pp->name)
			goto again;
		break;
	}
	for(i = 0; i < pp->numloc; i++) {
		if(nparams[i].type == ruledata && nparams[i].rulefree)
			free((void *)nparams[i].u.ruledata);
	}
	free(nparams);
	return rs;
}

static void
doassop(Expr *e, Cell *params)
{
	double val, *dst;

	if(e->left->op == TIDENT) {
		LGVAR(e->left,params)->u.table = LGVAR(e->right,params)->u.table;
		return;
	}

	val = eval(e->right, params);
	if(e->left->op == '[')
		dst = dereftab(LGVAR(e->left->left,params)->u.table,
					e->left->el, params);
	else
		dst = &LGVAR(e->left,params)->u.d;

	switch(e->op) {
	case '=':	*dst  = val;	break;
	case PLUSEQ:	*dst += val;	break;
	case MINUSEQ:	*dst -= val;	break;
	case TIMESEQ:	*dst *= val;	break;
	case DIVEQ:	*dst /= val;	break;
	case MODEQ:
		*dst = fmod(*dst, val);
		break;
	default:
		assert(0);
	}
}

static void
dostrass(SEXP *e, char *str, Cell *params)
{
	char *dst;

	//if(e->left->op == SIDENT) {
	//	LGVAR(e->left,params)->u.table = LGVAR(e->right,params)->u.table;
	//	return;
	//}

	dst = sexp(e, params);
        strncpy(dst, str, MAXSTRING);
}

static enum runstate
doforwhile(Statement *pc, Cell *params)
{
	double d;
	enum runstate rs;

	if(pc->code == FOR && (rs = exec(pc->forinit, params)) != normal)
		return rs;

	for(;;) {
		d = eval(pc->cond, params);
		if(d < 1e-15 && d > -1e-15)
			return normal;

		switch(rs = exec(pc->body1, params)) {
		case breaking:
			if(brkcont_lab == 0 || brkcont_lab == pc->label)
				return normal;
			return breaking;

		case conting:
			if(brkcont_lab == 0 || brkcont_lab == pc->label)
				continue;
			return conting;

		case looping:
		case returning:
			return rs;
                case normal:
                  	break;
		}

		if(pc->code == FOR)
			switch(rs = exec(pc->body2, params)) {
			case breaking:
				if(brkcont_lab == pc->label)
					return normal;
				return breaking;
			case conting:
				if(brkcont_lab == pc->label)
					continue;
				return conting;
			case looping:
			case returning:
				return rs;
                        case normal:
                         	 break;
			}
	}
	return normal;
}

enum runstate
doswitch(double d, Statement *pc, Cell *params)
{
    Cases *s = (Cases*)pc->body1;
/*     printf("Switch on %f\n", d); */
    while (s) {
/*       printf("Compare %f and %f\n", d, s->val); */
      if (d == s->val) {
        return exec(s->body, params);
      }
      s = s->next;
    }
/*     printf("Default\n"); */
    return exec(pc->body2, params);
}

enum runstate
exec(Statement *pc, Cell *params)
{
	double d;
	enum runstate rs;

	while(pc != 0) {
		rs = normal;
		switch(pc->code) {
		case CALL:
			rs = execproc(pc->proc, pc->el, params);
			if(rs == returning
			 &&brkcont_lab == (Symbol *)pc->cond) {
				rs = normal;
				brkcont_lab = 0;
			}
			break;

		case ELSE:
			d = eval(pc->cond, params);
			if(d > 1e-15 || d < -1e-15)
				rs = exec(pc->body1, params);
			else
				rs = exec(pc->body2, params);
			break;

		case IF:
			d = eval(pc->cond, params);
			if(d > 1e-15 || d < -1e-15)
				rs = exec(pc->body1, params);
			break;

                case SWITCH:
                        d = eval(pc->cond, params);
                        rs = doswitch(d, pc, params);
                        break;
		case END:
			printf("\nScript ending");
			tidy_up(1);
		case CLS:
			clearscreen();
			break;
		case FILLTABLE:
			filltable(LGVAR(pc->cond, params)->u.table,
                                  sexp(pc->string, params),
					pc->el, params);
			break;
		case FILLTABLEA:
			filltable(LGVAR(pc->cond, params)->u.table,
                                  mf_args(eval(pc->elr->e, params)),
                                  pc->el, params);
			break;
		case FILLTABLES:
                  filltables((TableS*)LGVAR(pc->cond, params)->u.table,
                             sexp(pc->string, params),
					pc->el, params);
			break;

		case FOR:
		case WHILE:
			rs = doforwhile(pc, params);
			break;

		case RULE:
                        
			(void)pc->rule->exec(pc,
					params,
					&params[pc->ruledataoffset].u.ruledata);
			break;

		case STOREFILE:
                  {
                    Sfile *s = sfile;
                    char *ss = sexp(pc->string, params);
                    while (s) {
                      /*printf("%p: Compare %d %d\n",
                             s, s->fnum, pc->ruledataoffset);*/
                      if (s->fnum == pc->ruledataoffset)
                        goto out;
                      s = s->next;
                    }
                    s = (Sfile*)malloc(sizeof(Sfile));
                    s->next = sfile; sfile = s;
                     if (pc->cond == NULL) {
                      s->storefile = malloc(strlen(ss)+1);
                      strcpy(s->storefile, ss);
                    }
                    else {      /* STOREFILEA */
                      char *a = mf_args(eval(pc->cond, params));
                      s->storefile = malloc(strlen(a)+1);
                      strcpy(s->storefile, a);
                    }
                    s->store_buf = (char *)calloc(sizeof(char), 2048);
                    if((s->storefd = open(s->storefile,
                                          O_CREAT | O_RDWR | O_TRUNC,
                                          0644)) < 0)
                      {
                        fprintf(stderr, "\nCan't open store-file %s",
                                s->storefile);
                        tidy_up(0);
                      }
                    s->fnum = pc->ruledataoffset;
                  out:
                    break;
                  }

		case STORSTR:
                  {
                    Sfile *s = sfile;
                    while (s) {
                      if (s->fnum == pc->ruledataoffset) break;
                      s = s->next;
                    }
                    if (s==NULL)
                      {
                        fprintf(stderr, "Storefile not allocated for storstr");
                        tidy_up(0);
                      }
                    if (pc->cond == NULL) {
                      strcat(s->store_buf, sexp(pc->string, params));
                    }
                    else {
                      char *a = mf_args(eval(pc->cond, params));
                      strcat(s->store_buf, a);
                    }
                    if(strlen(s->store_buf) > 1024) {
                      int check = write(s->storefd, s->store_buf, 1024);
                      if(check != 1024) {
                        fprintf(stderr, "\nSTORSTR: Storefile write failure!");
                        tidy_up(0);
                      }
                      memcpy(s->store_buf, s->store_buf + 1024, 1024);
                    }
                    break;
                  }
                case OPSYSCALL:
                  {
                    system(sexp(pc->string,params));
                    break;
                  }
                case SEND:
                  {
                    int size = 0;
                    Exprlist *el = pc->el;
                    do {        /* Cont number of arguments */
                      size++; el = el->next;
                    } while (el);
                    {
                      extern void do_send(char*, int, char*, double[], int);
                      double *vals = (double*)malloc(size*sizeof(double));
                      int cnt=0;

                      el = pc->el; /* reset to transfer data */
                      while((el=el->next)) {
                        vals[cnt++] = eval(el->e, NULL);
                      }
                      do_send(sexp(pc->string,params), (int)eval(pc->cond, params),
                              sexp((struct sexpr*)pc->body1, params), vals, cnt);
                      free(vals);
                    }
                    break;
                  }

                case DECDIG:
                        decdig = pc->ruledataoffset;
                        /*printf("DECDIG set to %d\n", decdig);*/
                        break;
		case '=':
			doassop(pc->cond, params);
			break;

                case '~':
                        dostrass((SEXP*)pc->el, sexp(pc->string,params), params);
                        break;

		case BREAK:
			rs = breaking;
			brkcont_lab = pc->label;
			break;

		case CONTINUE:
			rs = conting;
			brkcont_lab = pc->label;
			break;

		case LOOP:
			rs = looping;
			brkcont_lab = pc->label;
			break;

		case RETURN:
			rs = returning;
			brkcont_lab = pc->label;
			break;

		default:
			fprintf(stderr, "operation %d not defined yet\n", pc->code);
			tidy_up(0);
		}
		if(midi_used)
			midi_poll();
		if(rs != normal)
			return rs;
		pc = pc->next;
		if(finish)
			tidy_up(1);
	}
	return normal;
}

/*
 *	Table support/access routines
 */
void
mktable(Table *tab)
{
	int size = 1;
	int i;

        if (tab->tabdim.sizes[0]>=0) { /* Not for delayed tables */
          for(i = 0; i < tab->tabdim.dimensions; i++)
            size *= tab->tabdim.sizes[i];

          if(tab->mem == 0)
            tab->mem = (double *)emalloc(size * sizeof(double));

          for(i = 0; i < size; i++)
            tab->mem[i] = 0.0;
        }
}

double *
dereftab(Table *tab, Exprlist *idxs, Cell *params)
{
	int i;
	double *ptr;

	if(tab == 0) {
		fprintf(stderr, "TV: attempt to access null table variable\n");
		tidy_up(0);
	}
        if (tab->tabdim.sizes[0]<0) {
		fprintf(stderr, "TV: attempt to access empty table variable\n");
		tidy_up(0);
        }
	ptr = tab->mem;

	for(i = 0; i < tab->tabdim.dimensions; i++) {
		double didx = eval(idxs->e, params);
		int idx;

		if(!idxs->wrap) {
			didx -= floor(didx);
			idx = (int)(didx * (double)tab->tabdim.sizes[i]);
		} else {
			idx = (int)(floor(didx + 0.5));
/**/			if(idx < 0 || idx >= tab->tabdim.sizes[i]) {
				idx %= tab->tabdim.sizes[i];
				if(idx < 0)
					idx += tab->tabdim.sizes[i];
			}
		}
		ptr += idx * tab->tabdim.spaces[i];
		idxs = idxs->next;
	}
	return ptr;
}

char *
dereftabs(Table *tab, Exprlist *idxs, Cell *params)
{
	int i;
        int index = 0;
	char *ptr;

	if(tab == 0) {
		fprintf(stderr, "TV: attempt to access null table variable\n");
		tidy_up(0);
	}
        if (tab->tabdim.sizes[0]<0) {
		fprintf(stderr, "TV: attempt to access empty table variable\n");
		tidy_up(0);
        }
	ptr = (char *)tab->mem;

	for(i = 0; i < tab->tabdim.dimensions; i++) {
		double didx = eval(idxs->e, params);
		int idx;
                //printf("didx = %f\n", didx);
		if(!idxs->wrap) {
			didx -= floor(didx);
			idx = (int)(didx * (double)tab->tabdim.sizes[i]);
		} else {
			idx = (int)(floor(didx + 0.5));
/**/			if(idx < 0 || idx >= tab->tabdim.sizes[i]) {
				idx %= tab->tabdim.sizes[i];
				if(idx < 0)
					idx += tab->tabdim.sizes[i];
			}
		}
		index += idx * tab->tabdim.spaces[i];
		idxs = idxs->next;
	}
	return &ptr[index*MAXSTRING];
}

void
filltable(Table *tab, char *s, Exprlist *el, Cell *params)
{
	FILE	*fp;
	double	temp;
	char tempstr[161];
	char	str[30];
	int	i, j, offset = 0, size;
        int     defer = (tab->tabdim.sizes[0]<0); /* Deferred? */
        int     alloc;      /* current allocation */
        size = tab->tabdim.sizes[0];
	if(el != 0)
		offset = (int)(eval(el->e, params) + 0.5);

        if (defer) {
          tab->mem = (double*)malloc(1024 * sizeof(double));
          alloc = 1024;
          tab->tabdim.sizes[0] = size = alloc;
        }
        else {
          for(i = 0; i < tab->tabdim.dimensions; i++)
            size *= tab->tabdim.sizes[i];
        }

	fp = fopen(s, "r");
	if(fp == NULL) {
	    printf("\nCannot open file %s", s);
	    tidy_up(0);
	}

	i = 0; j = 0;

	while(fscanf(fp, "%s", str) != EOF) {
	    if(strncmp(str, "//", 2) != 0) {
              j++;
              temp = atof(str);
              if (defer && i>=alloc) {
                tab->mem = (double*)realloc(tab->mem, (alloc+=1024)*sizeof(double));
                  tab->tabdim.sizes[0] = size = alloc;
              }
              if((i < size) && (j >= offset))
                tab->mem[i++] = temp;
	    }
	    else {
		fgets(tempstr, 160, fp); /* find end-of-line */
	    }
	}
/*         printf("i=%d\n", i); */
        if (defer) {
          tab->mem = (double*)realloc(tab->mem, i*sizeof(double));
          tab->tabdim.sizes[0] = i;
        }
	fclose(fp);
}

void
filltables(TableS *tab, char *s, Exprlist *el, Cell *params)
{
	FILE	*fp;
        //	double	temp;
        //	char    tempstr[161];
	char	str[MAXSTRING];
	int	i, /*j, offset = 0, */size;
        int     defer = (tab->tabdim.sizes[0]<0); /* Deferred? */
        int     alloc;      /* current allocation */
        size = tab->tabdim.sizes[0];

	if(el != 0)
          (void)(eval(el->e, params) + 0.5);

        if (defer) {
          tab->mem = (char*)malloc(20 * MAXSTRING);
          alloc = 20;
          tab->tabdim.sizes[0] = size = alloc;
        }
        else {
          for(i = 0; i < tab->tabdim.dimensions; i++)
            size *= tab->tabdim.sizes[i];
        }

	fp = fopen(s, "r");
	if(fp == NULL) {
	    printf("\nCannot open file %s", s);
	    tidy_up(0);
	}

	i = 0; //j = 0;

	while(fgets(str,MAXSTRING, fp)) {
          if (defer && i>=alloc) {
            tab->mem = (char*)realloc(tab->mem, (alloc+=20)*MAXSTRING);
            tab->tabdim.sizes[0] = size = alloc;
          }
          if(i < size) {
            char *p = strchr(str, '\n');
            if (p) *p = '\0';
            strncpy(&tab->mem[MAXSTRING*i++], str, MAXSTRING-1);
          }
        }
/*         printf("i=%d\n", i); */
        if (defer) {
          tab->mem = (char*)realloc(tab->mem, i*MAXSTRING);
          tab->tabdim.sizes[0] = i;
        }
	fclose(fp);
}
