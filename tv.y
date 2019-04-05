/*
 *	Yacc grammar for Tabula Vigilans
 */
%{
#ifndef YYDEBUG
#define YYDEBUG	 1
#endif

#if YYDEBUG
extern int yydebug;
#endif

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "tv.h"

Expr *mkexpr(int op, Expr *left, Expr *right);
Local *mklocal(Symbol *name, enum celltype type, Expr *init);
Statement *mkstat(int op);
struct cases *mkcases(int op);
Statement *mkrule(Rule *rule, Exprlist *e1, Exprlist *e2);
extern Program *prog;

%}

%union {
	double number;
	struct symbol *symbol;
	struct expr *expr;
	struct exprlist *el;
	struct program *prog;
	struct proc *proc;
	struct statement *stat;
	struct param *pl;
	struct local *locals;
	struct rule *rule;
	struct mfunc *mfunc;
	struct table *tables;
	struct tabdim tabdim;
	struct cases *caseind;
	char *string;
	int code;
	struct sexpr *sexpr;
}

%start program

%left ','
%left ANDAND
%left OROR
%nonassoc '<' '>' '=' '!' LESS_EQ GTHAN_EQ EQ PLUSPLUS MINUSMINUS
%nonassoc PLUSEQ MINUSEQ TIMESEQ DIVEQ MODEQ
%left '+' '-'
%left '*' '/' '%'
%right '^'
%nonassoc UNARY_MINUS

%token	<number>	NUMBER
%token	<symbol>	TIDENT IDENT TABLE SIDENT STABLE
%token	<rule>		RULE
%token	<mfunc>		MFUNC
%token	<string>	STRING
%token	<sexp>		STRINGEXP

%token	CR
%token	IF ELSE END CLS CALL WHILE FOR LOCAL
%token	RETURN LOOP BREAK CONTINUE
%token	FILLTABLE STOREFILE STORSTR SEND DECDIG
%token	IMODO IMODC TRY SWITCH CASE DEFAULT
%token	FILLTABLEA ARGS OPSYSCALL GETARGSTR SHOWARGS
%token	FILLTABLES FILLTABLESA
%token	BEGCAT ENDCAT INT2STRING NUM2STRING

%type <prog>	program
%type <proc>	procs proc
%type <pl>	opt_param_list param_list param
%type <el>	opt_rexpr_list rexpr_list index_list index
%type <stat>	lines statement assignment filltable_stmt
%type <expr>	expression rexpression
%type <sexpr>	stringexp stringlist
%type <symbol>	opt_label opt_ident
%type <tables>	inits init
%type <tabdim>	tabdim dimension
%type <locals>	locals local
%type <code>	assop
%type <caseind> caseitem cases
%%
program:
	inits procs
	{
		prog = ALLOC(struct program);
		prog->tables = $1;
		prog->procs = $2;
		prog->globals = 0;
	}
;

inits:
	inits init
	{
		Table *s = $1;

		if(s == 0)
			$$ = $2;
		else {
			while(s->next != 0)
				s = s->next;
			s->next = $2;
		}
	}
|	/* */
	{ $$ = 0; }
;

init:
	TABLE TIDENT tabdim CR
	{
		$$ = ALLOC(Table);
		$$->name = $2;
		$$->tabdim = $3;
		$$->mem = 0;
		$$->next = NULL;
	}
|	TABLE SIDENT tabdim CR
	{
		$$ = ALLOC(Table);
		$$->name = $2;
		$$->tabdim = $3;
		$$->mem = 0;
		$$->next = NULL;
	}
;

tabdim:
	tabdim dimension
	{
	    if ($1.dimensions!=0xffffffff) {
		int *new = emalloc(($1.dimensions+1)*sizeof(int));

		memcpy(new, $1.sizes, $1.dimensions*sizeof(int));
		new[$1.dimensions] = $2.sizes[0];
		free($1.sizes);
		free($2.sizes);
		$$.dimensions = $1.dimensions + 1;
		$$.sizes = new;
	    }
	    else {		/* Deferred allocation */
	      free($1.sizes);
	      free($2.sizes);
	      $$.dimensions = $1.dimensions + 1;
	      $$.sizes = NULL;
	    }
	}
|	dimension
;

dimension:
	'[' NUMBER ']'
	{
		$$.dimensions = 1;
		$$.sizes = emalloc(sizeof(int));
		$$.sizes[0] = (int)($2+0.5);
	}
|	'[' ']'
	{
		$$.dimensions = 1;
		$$.sizes = emalloc(sizeof(int));
		$$.sizes[0] = (int)(0xffffffff);
	}
;

procs:
	procs proc
	{
		struct proc *p = $1;

		if(p == 0)
			$$ = $2;
		else {
			while(p->next != 0)
				p = p->next;
			p->next = $2;
		}
		
	}
|	/* */
	{ $$ = 0; }
;

proc:
	IDENT '(' opt_param_list ')' opt_cr '{' locals lines '}'
	{
		$$ = ALLOC(struct proc);
		$$->name = $1;
		$$->plist = $3;
		$$->locals = $7;
		$$->body = $8;
		$$->next = NULL;
		{
			int i = 0;
			Param *pl = $3;
			Local *ll = $7;
			while(pl != 0) {
				pl = pl->next;
				i++;
			}
			$$->arity = i;
			i = 0;
			while(ll != 0) {
				ll = ll->next;
				i++;
			}
			$$->numloc = i;
		}
	}
;

locals:
	locals local
	{
		Local *l = $1;

		if(l == 0)
			$$ = $2;
		else {
			while(l->next != 0)
				l = l->next;
			l->next = $2;
		}
	}
|	/* */
	{ $$ = 0; }
;

local:
	LOCAL IDENT opt_cr
	{
		$$ = mklocal($2, value, mkexpr(NUMBER, 0, 0));
		$$->init->value = 0.0;
	}
|	LOCAL IDENT '=' expression
	{
		$$ = mklocal($2, value, $4);
	}
|	LOCAL TIDENT opt_cr
	{
		$$ = mklocal($2, tableptr, 0);
	}
|	LOCAL TIDENT '=' TIDENT opt_cr
	{
		$$ = mklocal($2, tableptr, mkexpr(TIDENT, 0, 0));
		$$->init->sym = $4;
	}
;

lines:
	lines statement
	{
		struct statement *s = $1;

		while(s->next != 0)
			s = s->next;
		s->next = $2;
	}
|	statement
;

statement:
	IF '(' expression ')' opt_cr '{' lines '}'
	{
		$$ = mkstat(IF);
		$$->cond = $3;
		$$->body1 = $7;
	}
|	IF '(' expression ')' opt_cr '{' lines '}' ELSE '{' lines '}'
	{
		$$ = mkstat(ELSE);
		$$->cond = $3;
		$$->body1 = $7;
		$$->body2 = $11;
	}
|	CALL IDENT IDENT '(' opt_rexpr_list ')' opt_cr
	{
		$$ = mkstat(CALL);
		$$->cond = (Expr *)$2;	/* Yucc - fix this! */
		$$->label = $3;
		$$->el = $5;
	}
|	CALL IDENT '(' opt_rexpr_list ')' opt_cr
	{
		$$ = mkstat(CALL);
		$$->label = $2;
		$$->el = $4;
	}
|	opt_rexpr_list RULE stringexp ',' rexpr_list CR
	{
		Exprlist *el = ALLOC(Exprlist);
		el->e = ALLOC(Expr);
		el->e->op = STRING;
		el->e->string = $3;
		el->next = $5;
		$$ = mkrule($2, $1, el);
	}
|	opt_rexpr_list RULE stringexp opt_cr
	{
		Exprlist *el = ALLOC(Exprlist);
		el->e = ALLOC(Expr);
		el->e->op = STRING;
		el->e->string = $3;
		el->next = NULL;
		$$ = mkrule($2, $1, el);
	}
/* |	opt_rexpr_list RULE ARGS '(' expression ')' ',' rexpr_list CR */
/*	{ */
/*		Exprlist *el = ALLOC(Exprlist); */
/*		el->e = ALLOC(Expr); */
/*		el->e->op = STRING; */
/*		el->e->sym = lookup($5); */
/*		el->next = $8; */
/*		$$ = mkrule($2, $1, el); */
/*	} */
/* |	opt_rexpr_list RULE ARGS '(' expression ')' */
/*	{ */
/*		Exprlist *el = ALLOC(Exprlist); */
/*		el->e = ALLOC(Expr); */
/*		el->e->op = STRING; */
/*		el->e->sym = lookup($5); */
/*		el->next = NULL; */
/*		$$ = mkrule($2, $1, el); */
/*	} */
|	opt_rexpr_list RULE opt_rexpr_list CR
	{
		$$ = mkrule($2, $1, $3);
	}
|	BREAK opt_ident CR
	{
		$$ = mkstat(BREAK);
		$$->label = $2;
	}
|	CONTINUE opt_ident CR
	{
		$$ = mkstat(CONTINUE);
		$$->label = $2;
	}
|	LOOP opt_ident CR
	{
		$$ = mkstat(LOOP);
		$$->label = $2;
	}
|	RETURN opt_ident CR
	{
		$$ = mkstat(RETURN);
		$$->label = $2;
	}
|	END
	{
		$$ = mkstat(END);
	}
|	CLS
	{
		$$ = mkstat(CLS);
	}
|	WHILE opt_label '(' expression ')' opt_cr '{' lines '}'
	{
		$$ = mkstat(WHILE);
		$$->label = $2;
		$$->cond = $4;
		$$->body1 = $8;
	}
|	FOR opt_label '('
		assignment ';'
		expression ';'
		assignment ')' opt_cr '{' lines '}'
	{
		$$ = mkstat(FOR);
		$$->label = $2;
		$$->cond = $6;
		$$->forinit = $4;
		$$->body1 = $12;
		$$->body2 = $8;
	}
|	filltable_stmt
|	STOREFILE NUMBER stringexp opt_cr
	{
		$$ = mkstat(STOREFILE);
		$$->string = $3;
		$$->ruledataoffset = (int)($2+0.5);
	}
|	STOREFILE NUMBER ARGS '(' expression ')' CR
	{
		$$ = mkstat(STOREFILE);
		$$->cond = $5;
		$$->ruledataoffset = (int)($2+0.5);
	}
|	STOREFILE stringexp opt_cr
	{
		$$ = mkstat(STOREFILE);
		$$->string = $2;
		$$->ruledataoffset = 0;
	}
|	STOREFILE ARGS '(' expression ')' CR
	{
		$$ = mkstat(STOREFILE);
		$$->cond = $4;
		$$->ruledataoffset = 0;
	}
|	STORSTR NUMBER stringexp opt_cr
	{
		$$ = mkstat(STORSTR);
		$$->string = $3;
		$$->ruledataoffset = (int)($2+0.5);
	}
|	STORSTR NUMBER ARGS '(' expression ')' CR
	{
		$$ = mkstat(STORSTR);
		$$->cond = $5;
		$$->ruledataoffset = (int)($2+0.5);
	}
|	STORSTR stringexp opt_cr
	{
		$$ = mkstat(STORSTR);
		$$->string = $2;
		$$->ruledataoffset = 0;
	}
|	STORSTR ARGS '(' expression ')' CR
	{
		$$ = mkstat(STORSTR);
		$$->cond = $4;
		$$->ruledataoffset = 0;
	}
|	SHOWARGS ARGS '(' expression ')' CR
	{
		$$ = mkstat(SHOWARGS);
		$$->cond = $4;
		$$->ruledataoffset = 0;
		$$->next = NULL;
	}

|	OPSYSCALL stringexp opt_cr
	{
		$$ = mkstat(OPSYSCALL);
		$$->string = $2;
	}
|	SEND stringexp expression stringexp '(' rexpr_list ')' CR
	{
		$$ = mkstat(SEND);
		$$->string = $2;
		$$->cond = $3;
		$$->body1 = (struct statement*)$4;
		$$->el = $6;
		$$->ruledataoffset = 0;
	}
|	DECDIG NUMBER CR
	{
	    $$ = mkstat(DECDIG);
	    $$->ruledataoffset = (int)($2+0.5);
	}
|	assignment CR
|	stringexp '=' stringexp opt_cr
	{
	    $$ = mkstat('~');
	    $$->string = $3;
	    $$->el =(struct exprlist*)$1;
	}
|	SWITCH expression opt_cr '{' opt_cr cases opt_cr DEFAULT ':' opt_cr '{' lines opt_cr '}' opt_cr '}'
	{
	    /* $2 is var; $6 the cases, $12 the default */
	    $$ = mkstat(SWITCH);
	    $$->cond = $2;
	    $$->body1 = (Statement*)$6;
	    $$->body2 = $12;
	}
;

filltable_stmt:
	TIDENT FILLTABLE ARGS '(' expression ')' CR
	{
		$$ = mkstat(FILLTABLEA);
		$$->cond = mkexpr(TIDENT, 0, 0);
		$$->cond->sym = $1;
		$$->elr = ALLOC(Exprlist);
		$$->elr->e = $5;
		$$->elr->next = NULL;
		$$->el = NULL;
	}
|	TIDENT FILLTABLE stringexp opt_cr
	{
		$$ = mkstat(FILLTABLE);
		$$->cond = mkexpr(TIDENT, 0, 0);
		$$->cond->sym = $1;
		$$->string = $3;
		$$->el = NULL;
	}
|	TIDENT FILLTABLE stringexp ',' expression CR
	{
		$$ = mkstat(FILLTABLE);
		$$->cond = mkexpr(TIDENT, 0, 0);
		$$->cond->sym = $1;
		$$->string = $3;
		$$->el = ALLOC(Exprlist);
		$$->el->next = NULL;
		$$->el->e = $5;
	}
|	TIDENT FILLTABLE ARGS '(' expression ')' ',' expression CR
	{
		$$ = mkstat(FILLTABLEA);
		$$->cond = mkexpr(TIDENT, 0, 0);
		$$->cond->sym = $1;
		$$->elr = ALLOC(Exprlist);
		$$->elr->e = $5;
		$$->elr->next = NULL;
		$$->el = ALLOC(Exprlist);
		$$->el->next = NULL;
		$$->el->e = $8;
	}
|	SIDENT FILLTABLE stringexp opt_cr
	{
		$$ = mkstat(FILLTABLES);
		$$->cond = mkexpr(SIDENT, 0, 0);
		$$->cond->sym = $1;
		$$->string = $3;
		$$->el = NULL;
	}
|	SIDENT FILLTABLE ARGS '(' expression ')' CR
	{
		$$ = mkstat(FILLTABLESA);
		$$->cond = mkexpr(SIDENT, 0, 0);
		$$->cond->sym = $1;
		$$->elr = ALLOC(Exprlist);
		$$->elr->e = $5;
		$$->elr->next = NULL;
		$$->el = NULL;
	}
|	SIDENT FILLTABLE stringexp ',' expression CR
	{
		$$ = mkstat(FILLTABLES);
		$$->cond = mkexpr(SIDENT, 0, 0);
		$$->cond->sym = $1;
		$$->string = $3;
		$$->el = ALLOC(Exprlist);
		$$->el->next = NULL;
		$$->el->e = $5;
	}
|	SIDENT FILLTABLE ARGS '(' expression ')' ',' expression CR
	{
		$$ = mkstat(FILLTABLESA);
		$$->cond = mkexpr(SIDENT, 0, 0);
		$$->cond->sym = $1;
		$$->elr = ALLOC(Exprlist);
		$$->elr->e = $5;
		$$->elr->next = NULL;
		$$->el = ALLOC(Exprlist);
		$$->el->next = NULL;
		$$->el->e = $8;
	}
;

cases:	  cases caseitem {
		if ($1==NULL) $$ = $2;
		else {
		  Cases *s = $1;
		  while (s->next!=NULL) s = s->next;
		  s->next = $2; $$ = $1;
		}
	  }
|	/* */
	{ $$ = NULL; };

caseitem:   CASE NUMBER ':' opt_cr '{' lines opt_cr '}' opt_cr 
	{
	    $$ = mkcases(CASE);
	    $$->val = $2;
	    $$->body = $6;
	    $$->next = NULL;
	};

opt_ident:
	IDENT
|	/* */
	{ $$ = 0; }
;

assignment:
	rexpression assop rexpression
	{
		$$ = mkstat('=');
		$$->cond = mkexpr($2, $1, $3);	/* horrible use of cond */
	}
;

assop:
	'='		{ $$ = '='; }
|	PLUSEQ		{ $$ = PLUSEQ; }
|	MINUSEQ		{ $$ = MINUSEQ; }
|	TIMESEQ		{ $$ = TIMESEQ; }
|	DIVEQ		{ $$ = DIVEQ; }
|	MODEQ		{ $$ = MODEQ; }
;
		
opt_label:
	IDENT
|	/* */
	{ $$ = 0; }
;

opt_rexpr_list:
	rexpr_list
|	/* */
	{ $$ = 0; }
;

rexpr_list:
	rexpression
	{
		$$ = ALLOC(Exprlist);
		$$->e = $1;
		$$->next = NULL;
	}
|	rexpr_list ',' rexpression
	{
		Exprlist *e = $1;
		while(e->next != 0)
			e = e->next;
		e->next = ALLOC(Exprlist);
		e->next->e = $3;
		e->next->next = NULL;
	}
;

rexpression:
	TIDENT
	{
		$$ = mkexpr(TIDENT, 0, 0);
		$$->sym = $1;
	}
|	SIDENT
	{
		$$ = mkexpr(SIDENT, 0, 0);
		$$->sym = $1;
	}
|	expression
;

opt_param_list:
	param_list
|	/* */
	{ $$ = 0; }
;

param_list:
	param
|	param_list ',' param
	{
		struct param *p = $1;
		while(p->next != 0)
			p = p->next;
		p->next = $3;
	}
;

param:
	IDENT
	{
		$$ = ALLOC(Param);
		$$->sym = $1;
		$$->next = NULL;
	}
|	TIDENT
	{
		$$ = ALLOC(Param);
		$$->sym = $1;
		$$->next = NULL;
	}
|	SIDENT
	{
		$$ = ALLOC(Param);
		$$->sym = $1;
		$$->next = NULL;
	}
;

expression:
	'(' expression ')'
	{ $$ = $2; }
|	expression ANDAND expression
	{ $$ = mkexpr(ANDAND, $1, $3); }
|	expression OROR expression
	{ $$ = mkexpr(OROR, $1, $3); }
|	expression '<' expression
	{ $$ = mkexpr('<', $1, $3); }
|	expression '>' expression
	{ $$ = mkexpr('>', $1, $3); }
|	expression EQ expression
	{ $$ = mkexpr(EQ, $1, $3); }
|	expression '!' expression
	{ $$ = mkexpr('!', $1, $3); }
|	TIDENT EQ TIDENT
	{
		$$ = mkexpr(EQ, mkexpr(TIDENT, 0, 0), mkexpr(TIDENT, 0, 0));
		$$->left->sym = $1;
		$$->right->sym = $3;
	}
|	TIDENT '!' TIDENT
	{
		$$ = mkexpr('!', mkexpr(TIDENT, 0, 0), mkexpr(TIDENT, 0, 0));
		$$->left->sym = $1;
		$$->right->sym = $3;
	}
|	expression LESS_EQ expression
	{ $$ = mkexpr(LESS_EQ, $1, $3); }
|	expression GTHAN_EQ expression
	{ $$ = mkexpr(GTHAN_EQ, $1, $3); }
|	expression '+' expression
	{ $$ = mkexpr('+', $1, $3); }
|	expression '-' expression
	{ $$ = mkexpr('-', $1, $3); }
|	expression '*' expression
	{ $$ = mkexpr('*', $1, $3); }
|	expression '/' expression
	{ $$ = mkexpr('/', $1, $3); }
|	expression '%' expression
	{ $$ = mkexpr('%', $1, $3); }
|	expression '^' expression
	{ $$ = mkexpr('^', $1, $3); }
|	'-' expression			%prec UNARY_MINUS
	{ $$ = mkexpr(UNARY_MINUS, $2, 0); }
|	PLUSPLUS expression
	{ $$ = mkexpr(PLUSPLUS, $2, 0); }
|	MINUSMINUS expression
	{ $$ = mkexpr(MINUSMINUS, $2, 0); }
|	expression PLUSPLUS
	{ $$ = mkexpr(PLUSPLUS+POST, $1, 0); }
|	expression MINUSMINUS
	{ $$ = mkexpr(MINUSMINUS+POST, $1, 0); }
	
|	IDENT
	{
		$$ = mkexpr(IDENT, 0, 0);
		$$->sym = $1;
	}
|	NUMBER
	{
		$$ = mkexpr(NUMBER, 0, 0);
		$$->value = $1;
	}
|	MFUNC '(' ')'
	{
		$$ = mkexpr(MFUNC, 0, 0);
		$$->mfunc = $1;
	}
|	MFUNC '(' rexpression ')'
	{
		$$ = mkexpr(MFUNC, $3, 0);
		$$->mfunc = $1;
	}
|	MFUNC '(' rexpression ',' expression ')'
	{
		$$ = mkexpr(MFUNC, $3, $5);
		$$->mfunc = $1;
	}
|	TIDENT index_list
	{
		$$ = mkexpr('[', mkexpr(TIDENT, 0, 0), 0);
		$$->left->sym = $1;
		$$->el = $2;
	}
|	TRY '(' opt_rexpr_list RULE STRING ',' rexpr_list ')'
	{
		Exprlist *el = ALLOC(Exprlist);
		el->e = ALLOC(Expr);
		el->e->op = STRING;
		el->e->sym = lookup($5);
		el->next = $7;
		$$ = mkexpr(TRY, 0, 0);
		$$->rule = mkrule($4, $3, el);
	}
|	TRY '(' opt_rexpr_list RULE STRING ')'
	{
		Exprlist *el = ALLOC(Exprlist);
		el->e = ALLOC(Expr);
		el->e->op = STRING;
		el->e->sym = lookup($5);
		el->next = NULL;
		$$ = mkexpr(TRY, 0, 0);
		$$->rule = mkrule($4, $3, el);
	}
|	TRY '(' opt_rexpr_list RULE opt_rexpr_list ')'
	{
		$$ = mkexpr(TRY, 0, 0);
		$$->rule = mkrule($4, $3, $5);
	}
;

index_list:
	index_list index
	{
		Exprlist *e = $1;

		while(e->next != 0)
			e = e->next;
		e->next = $2;
	}
|	index
;

index:
	'[' expression ']'
	{
		$$ = ALLOC(Exprlist);
		$$->e = $2;
		$$->wrap = 1;
		$$->next = NULL;
	}
|	IMODO expression IMODC
	{
		$$ = ALLOC(Exprlist);
		$$->e = $2;
		$$->wrap = 0;
		$$->next = NULL;
	}
;

opt_cr:
	CR
|	/* */
;

stringexp:
	SIDENT index_list
	{
		 $$ = ALLOC(SEXP);
		 $$->op = '[';
		 $$->sym = $1;
		 $$->el = $2;
		 $$->string = NULL;
	}
|	STRING
	{
	    $$ = ALLOC(SEXP);
	    $$->op = 0;
	    $$->sym = NULL;
	    $$->el = NULL;
	    $$->string = $1;
	}
|	BEGCAT stringlist ENDCAT
	{
	    $$ = ALLOC(SEXP);
	    $$->op = BEGCAT;
	    $$->next = $2;
	}
|	INT2STRING '(' expression ')'
	{
		$$ = ALLOC(SEXP);
		$$->op = INT2STRING;
		$$->el = ALLOC(Exprlist);
		$$->el->e = $3;
		$$->el->wrap = 0;
		$$->el->next = 0;
		$$->next = NULL;
	}
|	NUM2STRING '(' expression ')'
	{
	    $$ = ALLOC(SEXP);
	    $$->op = NUM2STRING;
		$$->el = ALLOC(Exprlist);
		$$->el->e = $3;
		$$->el->wrap = 0;
		$$->el->next = 0;
		$$->next = NULL;
	}
;

stringlist:
	stringexp
	{
		$$ = $1;
		$$->next = NULL;
	}
|	stringlist ',' stringexp
	{
		SEXP *e = $1;
		while(e->next != 0)
			e = e->next;
		e->next = $3;
	}
;

%%

void
yyerror(char *str)
{
	fprintf(stderr, "line %d: %s\n", linenum, str);
}

void *
emalloc(unsigned int s)
{
	void *r;

	if((r = calloc(1, s)) == NULL) {
		fprintf(stderr, "Can't allocate %d bytes of memory\n", s);
		tidy_up(0);
	}
	return r;
}

Expr *
mkexpr(int op, Expr *left, Expr *right)
{
	Expr *r = ALLOC(Expr);

	r->op = op;
	r->left = left;
	r->right = right;
	return r;
}

Local *
mklocal(Symbol *name, enum celltype type, Expr *init)
{
	struct local *r = ALLOC(Local);
	r->name = name;
	r->type = value;
	r->init = init;
	r->next = NULL;
	return r;
}

struct statement *
mkstat(int op)
{
	struct statement *s = ALLOC(Statement);

	s->code = op;
	s->next = NULL;
	return s;
}

struct cases *
mkcases(int op)
{
	struct cases *s = ALLOC(Cases);

	s->code = op;
	s->next = NULL;
	return s;
}

int
ellen(Exprlist *el)
{
	int i = 0;

	while(el != 0) {
		i++;
		el = el->next;
	}
	return i;
}

struct statement *
mkrule(Rule *rule, Exprlist *e1, Exprlist *e2)
{
	int e1l = ellen(e1);
	int e2l = ellen(e2);
	struct statement *s = mkstat(RULE);

	if(rule->left_arity >= 0 && e1l != rule->left_arity) {
	    if(rule->left_arity == 0)
		fprintf(stderr, "TV: rule '%s' should have no output parameters\n",
				rule->picture);
	    else if(rule->left_arity == 1)
		fprintf(stderr, "TV: rule '%s' should have one output parameter\n",
				rule->picture);
	    else
		fprintf(stderr, "TV: rule '%s' requires %d output parameters\n",
				rule->picture, rule->left_arity);
		tidy_up(0);
	}

	if(rule->right_arity >= 0 && e2l != rule->right_arity) {
	    if(rule->right_arity == 0)
		fprintf(stderr, "TV: rule '%s' should have no input parameters\n",
					rule->picture);
	    else if(rule->right_arity == 1)
		fprintf(stderr, "TV: rule '%s' should have one input parameter\n",
					rule->picture);
	    else
		fprintf(stderr, "TV: rule '%s' requires %d input parameters\n",
					rule->picture, rule->right_arity);
		tidy_up(0);
	}

	if(rule->total_arity >= 0 && e1l+e2l != rule->total_arity) {
		fprintf(stderr, "TV: rule '%s' requires %d parameters in total\n",
					rule->picture, rule->total_arity);
		tidy_up(0);
	}

	s->el = e1;
	s->elr = e2;
	s->rule = rule;
	return s;
}

