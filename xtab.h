/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ANDAND = 258,
    OROR = 259,
    LESS_EQ = 260,
    GTHAN_EQ = 261,
    EQ = 262,
    PLUSPLUS = 263,
    MINUSMINUS = 264,
    PLUSEQ = 265,
    MINUSEQ = 266,
    TIMESEQ = 267,
    DIVEQ = 268,
    MODEQ = 269,
    UNARY_MINUS = 270,
    NUMBER = 271,
    TIDENT = 272,
    IDENT = 273,
    TABLE = 274,
    SIDENT = 275,
    STABLE = 276,
    RULE = 277,
    MFUNC = 278,
    STRING = 279,
    STRINGEXP = 280,
    CR = 281,
    IF = 282,
    ELSE = 283,
    END = 284,
    CLS = 285,
    CALL = 286,
    WHILE = 287,
    FOR = 288,
    LOCAL = 289,
    RETURN = 290,
    LOOP = 291,
    BREAK = 292,
    CONTINUE = 293,
    FILLTABLE = 294,
    STOREFILE = 295,
    STORSTR = 296,
    SEND = 297,
    DECDIG = 298,
    IMODO = 299,
    IMODC = 300,
    TRY = 301,
    SWITCH = 302,
    CASE = 303,
    DEFAULT = 304,
    FILLTABLEA = 305,
    ARGS = 306,
    OPSYSCALL = 307,
    OPSYSCALLS = 308,
    GETARGSTR = 309,
    FILLTABLES = 310,
    FILLTABLESA = 311,
    BEGCAT = 312,
    ENDCAT = 313,
    INT2STRING = 314,
    NUM2STRING = 315
  };
#endif
/* Tokens.  */
#define ANDAND 258
#define OROR 259
#define LESS_EQ 260
#define GTHAN_EQ 261
#define EQ 262
#define PLUSPLUS 263
#define MINUSMINUS 264
#define PLUSEQ 265
#define MINUSEQ 266
#define TIMESEQ 267
#define DIVEQ 268
#define MODEQ 269
#define UNARY_MINUS 270
#define NUMBER 271
#define TIDENT 272
#define IDENT 273
#define TABLE 274
#define SIDENT 275
#define STABLE 276
#define RULE 277
#define MFUNC 278
#define STRING 279
#define STRINGEXP 280
#define CR 281
#define IF 282
#define ELSE 283
#define END 284
#define CLS 285
#define CALL 286
#define WHILE 287
#define FOR 288
#define LOCAL 289
#define RETURN 290
#define LOOP 291
#define BREAK 292
#define CONTINUE 293
#define FILLTABLE 294
#define STOREFILE 295
#define STORSTR 296
#define SEND 297
#define DECDIG 298
#define IMODO 299
#define IMODC 300
#define TRY 301
#define SWITCH 302
#define CASE 303
#define DEFAULT 304
#define FILLTABLEA 305
#define ARGS 306
#define OPSYSCALL 307
#define OPSYSCALLS 308
#define GETARGSTR 309
#define FILLTABLES 310
#define FILLTABLESA 311
#define BEGCAT 312
#define ENDCAT 313
#define INT2STRING 314
#define NUM2STRING 315

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 34 "tv.y" /* yacc.c:1909  */

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

#line 194 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
