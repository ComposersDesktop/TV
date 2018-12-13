/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 4 "tv.y" /* yacc.c:339  */

#ifndef YYDEBUG
#define YYDEBUG	 1
#endif

#if YYDEBUG
extern int yydebug;
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef LATTICE
#include <osbind.h>
#include <gemlib.h>
#endif
#ifndef	LATTICE
#include <malloc.h>
#endif
#include <memory.h>
#include "tv.h"

Expr *mkexpr(int op, Expr *left, Expr *right);
Local *mklocal(Symbol *name, enum celltype type, Expr *init);
Statement *mkstat(int op);
struct cases *mkcases(int op);
Statement *mkrule(Rule *rule, Exprlist *e1, Exprlist *e2);
extern Program *prog;


#line 96 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 34 "tv.y" /* yacc.c:355  */

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

#line 276 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 293 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1399

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  80
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  338

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   315

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     9,     2,     2,     2,    24,     2,     2,
      74,    75,    22,    20,     3,    21,     2,    23,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    79,    78,
       6,     8,     7,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    72,     2,    73,    25,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    76,     2,    77,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     4,     5,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    97,    97,   107,   120,   124,   132,   143,   162,   166,
     172,   181,   195,   199,   227,   240,   244,   249,   253,   257,
     265,   273,   277,   283,   290,   297,   303,   312,   339,   343,
     348,   353,   358,   363,   367,   371,   378,   390,   391,   397,
     403,   409,   415,   421,   427,   433,   439,   444,   453,   458,
     459,   465,   476,   486,   494,   504,   516,   524,   534,   544,
     558,   567,   569,   578,   580,   584,   592,   593,   594,   595,
     596,   597,   601,   603,   607,   609,   613,   619,   631,   636,
     641,   645,   647,   651,   652,   662,   668,   674,   683,   685,
     687,   689,   691,   693,   695,   697,   703,   709,   711,   713,
     715,   717,   719,   721,   723,   725,   727,   729,   731,   733,
     736,   741,   746,   751,   756,   761,   767,   777,   787,   795,
     803,   807,   814,   824,   825,   829,   837,   845,   851,   861,
     874,   879
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "','", "ANDAND", "OROR", "'<'", "'>'",
  "'='", "'!'", "LESS_EQ", "GTHAN_EQ", "EQ", "PLUSPLUS", "MINUSMINUS",
  "PLUSEQ", "MINUSEQ", "TIMESEQ", "DIVEQ", "MODEQ", "'+'", "'-'", "'*'",
  "'/'", "'%'", "'^'", "UNARY_MINUS", "NUMBER", "TIDENT", "IDENT", "TABLE",
  "SIDENT", "STABLE", "RULE", "MFUNC", "STRING", "STRINGEXP", "CR", "IF",
  "ELSE", "END", "CLS", "CALL", "WHILE", "FOR", "LOCAL", "RETURN", "LOOP",
  "BREAK", "CONTINUE", "FILLTABLE", "STOREFILE", "STORSTR", "SEND",
  "DECDIG", "IMODO", "IMODC", "TRY", "SWITCH", "CASE", "DEFAULT",
  "FILLTABLEA", "ARGS", "OPSYSCALL", "OPSYSCALLS", "GETARGSTR",
  "FILLTABLES", "FILLTABLESA", "BEGCAT", "ENDCAT", "INT2STRING",
  "NUM2STRING", "'['", "']'", "'('", "')'", "'{'", "'}'", "';'", "':'",
  "$accept", "program", "inits", "init", "tabdim", "dimension", "procs",
  "proc", "locals", "local", "lines", "statement", "filltable_stmt",
  "cases", "caseitem", "opt_ident", "assignment", "assop", "opt_label",
  "opt_rexpr_list", "rexpr_list", "rexpression", "opt_param_list",
  "param_list", "param", "expression", "index_list", "index", "opt_cr",
  "stringexp", "stringlist", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    44,   258,   259,    60,    62,    61,    33,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
      43,    45,    42,    47,    37,    94,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,    91,    93,    40,    41,   123,   125,    59,    58
};
# endif

#define YYPACT_NINF -224

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-224)))

#define YYTABLE_NINF -125

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-125)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -224,    15,    -8,  -224,    43,  -224,    12,   -17,   -17,    -5,
    -224,    -9,   -20,  -224,     3,   131,    20,  -224,  -224,  -224,
    -224,  -224,  -224,  -224,    57,   127,  -224,  -224,   101,   131,
    -224,    68,  -224,  -224,   974,   178,   178,   178,  -224,   106,
    -224,   102,    72,  -224,    74,  -224,  -224,   120,   125,   125,
      51,   129,   129,   129,   129,  1051,  1108,   245,   136,    90,
     178,   245,   245,    93,    94,   178,  -224,   681,  -224,  -224,
     139,   140,   174,   304,  1291,   179,    61,  1354,  1354,  -224,
     154,   161,   148,   178,   178,    73,  -224,   180,    73,   936,
     178,   -18,  -224,   116,   121,    35,    99,  -224,   157,   160,
     163,   177,   270,    73,   143,   101,   909,   146,   101,   178,
     184,  1089,  1181,   101,  -224,     9,   178,   178,   286,  -224,
    -224,  -224,  1070,  1089,  -224,  -224,  -224,  -224,  -224,  -224,
    1089,   178,   178,   178,   178,   178,   178,   178,   178,  -224,
    -224,   178,   178,   178,   178,   178,   178,   245,  -224,  -224,
     162,    23,  1144,    24,  -224,   167,    55,    61,  -224,  -224,
       2,   342,   169,  1089,   178,  1089,   194,  -224,   178,  -224,
    -224,  -224,  -224,  -224,   172,   101,   178,  -224,   173,   101,
     178,  -224,   592,  -224,   204,  -224,   181,  -224,   245,  -224,
     364,   386,  -224,    73,   221,    85,  -224,  -224,  1334,  1374,
    1354,  1354,  1354,  1354,  1354,  1354,   208,   208,   234,   234,
     234,   234,   101,   178,   178,  -224,  -224,  -224,   178,   178,
    -224,   178,  -224,   101,  1089,   188,   409,   186,   304,   101,
    1313,   178,  -224,   431,   178,  -224,   453,   203,   618,   101,
    -224,  -224,  -224,  -224,  1089,  -224,  -224,   476,  1203,   498,
    1225,   520,   202,   212,   101,   101,   178,  -224,   543,   242,
     565,   251,  1089,     6,   214,  -224,    88,    92,  -224,   114,
    -224,  -224,  1022,   101,  -224,   226,   261,   266,  -224,   267,
    -224,     7,  1089,  -224,  -224,    28,  -224,   178,  -224,   178,
    -224,   732,  -224,  1022,  1089,  -224,  -224,   277,    10,   290,
    -224,   258,  1247,  1269,   285,   783,   250,  -224,  -224,   247,
     248,  -224,  -224,   252,  -224,   101,   101,   101,  1022,   254,
     255,   257,   834,  1022,  1022,  1022,  -224,   885,   630,   630,
    -224,   259,   260,   101,   101,  -224,   265,  -224
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,    12,     1,     0,     3,     2,     0,     0,     0,
      11,     0,     0,     8,     0,    82,     0,    10,     5,     7,
       6,    86,    85,    87,     0,    81,    83,     9,   124,     0,
     123,     0,    84,    15,    75,     0,     0,     0,   111,    78,
     110,    79,     0,   126,     0,    33,    34,     0,    73,    73,
       0,    64,    64,    64,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    75,    21,    37,
       0,     0,    74,    76,    80,     0,     0,   106,   107,   105,
       0,     0,     0,     0,     0,   115,   120,     0,   125,     0,
       0,     0,    72,     0,     0,   124,   124,    63,     0,     0,
       0,     0,     0,     0,     0,   124,     0,     0,   124,     0,
       0,    75,   124,   124,   130,     0,     0,     0,     0,    13,
      20,    49,    75,     0,    66,    67,    68,    69,    70,    71,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   108,
     109,     0,     0,     0,     0,     0,     0,     0,    96,    95,
       0,   124,     0,     0,   119,     0,   124,    78,    79,   112,
       0,     0,     0,    75,     0,     0,     0,    18,     0,    16,
      32,    31,    29,    30,     0,   124,     0,    40,     0,   124,
       0,    44,     0,    48,     0,    76,     0,    46,     0,   127,
       0,     0,    88,    79,     0,   124,    77,    65,    89,    90,
      91,    92,    94,    97,    98,    93,    99,   100,   101,   102,
     103,   104,   124,     0,     0,    53,   122,   121,     0,     0,
      56,     0,   113,   124,    75,     0,     0,     0,     0,   124,
      17,     0,    38,     0,     0,    42,     0,     0,    75,   124,
     131,   128,   129,    28,     0,    27,    50,     0,     0,     0,
       0,     0,     0,     0,   124,   124,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,    54,     0,
      58,   114,    75,   124,    25,     0,     0,     0,    41,     0,
      45,     0,     0,   117,   118,   124,    26,     0,    52,     0,
      57,    75,    24,    75,     0,    39,    43,     0,     0,     0,
      60,     0,     0,     0,    22,    75,     0,    47,   116,     0,
       0,    55,    59,     0,    35,   124,   124,   124,    75,     0,
       0,     0,    75,    75,    75,    75,    23,    75,    75,    75,
      36,     0,     0,   124,   124,    62,     0,    51
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -224,  -224,  -224,  -224,   326,   112,  -224,  -224,  -224,  -224,
    -204,   -67,  -224,  -224,  -224,   118,  -157,  -224,   294,   -97,
    -223,   -69,  -224,  -224,   315,   -33,   -40,    54,   -89,   147,
    -224
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     5,    12,    13,     6,    10,    34,    66,
      67,    68,    69,   285,   300,    98,    70,   130,    93,    71,
      72,    73,    24,    25,    26,    74,    85,    86,    31,    75,
     115
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     120,    88,    77,    78,    79,   221,   167,   169,   227,   282,
     123,   162,   188,   123,   184,     3,   177,    18,    16,   181,
     160,   266,     4,   186,   187,   194,   214,   112,   131,   132,
     133,   134,   118,   135,   136,   137,   138,   139,   140,   281,
      20,     9,   185,   166,   141,   142,   143,   144,   145,   146,
     152,   153,    11,   185,   196,    11,   163,   161,   219,   298,
      30,   197,   215,    88,    17,    30,   225,   220,   291,    15,
      80,     7,    30,    81,     8,    11,   182,   222,   189,    95,
      96,   283,   297,   190,   191,   308,   232,   299,   244,   305,
     235,   123,    30,    27,   185,   287,   228,   217,   198,   199,
     200,   201,   202,   203,   204,   205,   245,   168,   206,   207,
     208,   209,   210,   211,   322,    80,    83,   289,    81,   327,
     328,   329,    30,   246,    19,   286,    19,   253,    83,   288,
      29,   226,    28,    84,   252,   230,    30,   306,    30,   154,
     257,   264,   154,   233,    33,    84,    89,   236,    90,    91,
     265,   290,    87,    88,    92,   185,    82,    83,    97,    21,
      22,    83,    23,   110,   111,   274,   275,   116,   117,   185,
      99,   100,   101,   122,    84,   185,   121,   123,    84,   103,
     247,   248,   148,    43,   292,   249,   250,   147,   251,   149,
     164,    35,    36,   185,   170,   165,   301,   171,   258,    37,
     172,   260,   105,   108,   109,    38,    76,    40,   113,   114,
     150,   103,    42,   185,   173,    43,    62,   176,    63,    64,
     180,   183,   229,   276,   120,   228,   319,   320,   321,   151,
     143,   144,   145,   146,   156,    59,   213,   238,   120,   331,
     332,   218,   155,   224,   335,   336,   231,   234,    62,   175,
      63,    64,    65,   179,   302,   120,   303,   239,   243,   146,
     120,   120,   120,   254,   256,   131,   132,   133,   134,   195,
     135,   136,   137,   138,   139,   140,   103,   262,   272,   278,
      43,   141,   142,   143,   144,   145,   146,   273,   280,   284,
     131,   132,   133,   134,   212,   135,   136,   137,   138,   139,
     140,   103,   293,   295,   296,    43,   141,   142,   143,   144,
     145,   146,   124,    62,   307,    63,    64,   309,   310,   125,
     126,   127,   128,   129,   313,   315,   316,   317,   318,   237,
     323,   324,   174,   325,    14,   240,   333,   334,    62,   294,
      63,    64,   337,    94,    32,     0,   131,   132,   133,   134,
       0,   135,   136,   137,   138,   139,   140,     0,     0,     0,
       0,   192,   141,   142,   143,   144,   145,   146,   131,   132,
     133,   134,     0,   135,   136,   137,   138,   139,   140,     0,
       0,     0,     0,     0,   141,   142,   143,   144,   145,   146,
     131,   132,   133,   134,     0,   135,   136,   137,   138,   139,
     140,     0,     0,     0,     0,     0,   141,   142,   143,   144,
     145,   146,     0,   131,   132,   133,   134,   223,   135,   136,
     137,   138,   139,   140,     0,     0,     0,     0,     0,   141,
     142,   143,   144,   145,   146,   131,   132,   133,   134,   241,
     135,   136,   137,   138,   139,   140,     0,     0,     0,     0,
       0,   141,   142,   143,   144,   145,   146,   131,   132,   133,
     134,   242,   135,   136,   137,   138,   139,   140,     0,     0,
       0,     0,     0,   141,   142,   143,   144,   145,   146,     0,
     131,   132,   133,   134,   255,   135,   136,   137,   138,   139,
     140,     0,     0,     0,     0,     0,   141,   142,   143,   144,
     145,   146,   131,   132,   133,   134,   259,   135,   136,   137,
     138,   139,   140,     0,     0,     0,     0,     0,   141,   142,
     143,   144,   145,   146,   131,   132,   133,   134,   261,   135,
     136,   137,   138,   139,   140,     0,     0,     0,     0,     0,
     141,   142,   143,   144,   145,   146,     0,   131,   132,   133,
     134,   267,   135,   136,   137,   138,   139,   140,     0,     0,
       0,     0,     0,   141,   142,   143,   144,   145,   146,   131,
     132,   133,   134,   269,   135,   136,   137,   138,   139,   140,
       0,     0,     0,     0,     0,   141,   142,   143,   144,   145,
     146,     0,     0,     0,     0,   271,   131,   132,   133,   134,
       0,   135,   136,   137,   138,   139,   140,     0,     0,     0,
       0,     0,   141,   142,   143,   144,   145,   146,   277,     0,
       0,     0,     0,   103,     0,     0,     0,    43,     0,     0,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    37,
     279,     0,     0,    35,    36,    38,   157,    40,     0,   158,
       0,    37,    42,   263,     0,     0,     0,    38,    39,    40,
      62,    41,    63,    64,    42,    43,     0,    30,    44,     0,
      45,    46,    47,    48,    49,    59,    51,    52,    53,    54,
       0,    55,    56,    57,    58,     0,     0,    59,    60,     0,
       0,     0,    65,    61,    35,    36,     0,     0,    62,     0,
      63,    64,    37,     0,    65,     0,     0,  -124,    38,    39,
      40,     0,    41,     0,     0,    42,    43,     0,     0,    44,
       0,    45,    46,    47,    48,    49,     0,    51,    52,    53,
      54,     0,    55,    56,    57,    58,     0,     0,    59,    60,
       0,     0,     0,     0,    61,    35,    36,     0,     0,    62,
       0,    63,    64,    37,     0,    65,     0,     0,   119,    38,
      39,    40,     0,    41,     0,     0,    42,    43,     0,     0,
      44,     0,    45,    46,    47,    48,    49,     0,    51,    52,
      53,    54,     0,    55,    56,    57,    58,     0,     0,    59,
      60,     0,     0,     0,     0,    61,    35,    36,     0,     0,
      62,     0,    63,    64,    37,     0,    65,     0,     0,   304,
      38,    39,    40,     0,    41,     0,     0,    42,    43,     0,
       0,    44,     0,    45,    46,    47,    48,    49,     0,    51,
      52,    53,    54,     0,    55,    56,    57,    58,     0,     0,
      59,    60,     0,     0,     0,     0,    61,    35,    36,     0,
       0,    62,     0,    63,    64,    37,     0,    65,     0,     0,
     314,    38,    39,    40,     0,    41,     0,     0,    42,    43,
       0,     0,    44,     0,    45,    46,    47,    48,    49,     0,
      51,    52,    53,    54,     0,    55,    56,    57,    58,     0,
       0,    59,    60,     0,     0,     0,     0,    61,    35,    36,
       0,     0,    62,     0,    63,    64,    37,     0,    65,     0,
       0,   326,    38,    39,    40,     0,    41,     0,     0,    42,
      43,     0,     0,    44,     0,    45,    46,    47,    48,    49,
       0,    51,    52,    53,    54,     0,    55,    56,    57,    58,
     103,     0,    59,    60,    43,     0,     0,     0,    61,    35,
      36,     0,     0,    62,     0,    63,    64,    37,     0,    65,
       0,     0,   330,    38,   157,    40,     0,   158,     0,     0,
      42,   178,     0,     0,     0,     0,     0,    62,     0,    63,
      64,     0,     0,     0,     0,     0,     0,    35,    36,     0,
       0,     0,     0,    59,     0,    37,     0,     0,     0,     0,
       0,    38,    39,    40,     0,    41,     0,     0,    42,    43,
      65,   159,    44,     0,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,     0,    55,    56,    57,    58,     0,
       0,    59,    60,     0,     0,    35,    36,    61,     0,     0,
       0,     0,    62,    37,    63,    64,     0,     0,    65,    38,
      39,    40,     0,    41,     0,     0,    42,    43,     0,     0,
      44,     0,    45,    46,    47,    48,    49,     0,    51,    52,
      53,    54,     0,    55,    56,    57,    58,     0,   102,    59,
      60,     0,   103,    35,    36,    61,    43,     0,     0,     0,
      62,    37,    63,    64,     0,     0,    65,    38,   157,    40,
       0,   193,    35,    36,    42,    43,     0,     0,     0,     0,
      37,     0,     0,   104,     0,     0,    38,   157,    40,    62,
     158,    63,    64,    42,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,   106,     0,     0,    62,   103,
      63,    64,     0,    43,    65,     0,    59,     0,   131,   132,
     133,   134,     0,   135,   136,   137,   138,   139,   140,     0,
       0,     0,     0,    65,   141,   142,   143,   144,   145,   146,
     107,     0,     0,     0,     0,     0,    62,     0,    63,    64,
       0,     0,     0,     0,     0,   131,   132,   133,   134,     0,
     135,   136,   137,   138,   139,   140,     0,     0,     0,     0,
     216,   141,   142,   143,   144,   145,   146,   131,   132,   133,
     134,     0,   135,   136,   137,   138,   139,   140,    30,     0,
       0,     0,     0,   141,   142,   143,   144,   145,   146,   131,
     132,   133,   134,     0,   135,   136,   137,   138,   139,   140,
     268,     0,     0,     0,     0,   141,   142,   143,   144,   145,
     146,   131,   132,   133,   134,     0,   135,   136,   137,   138,
     139,   140,   270,     0,     0,     0,     0,   141,   142,   143,
     144,   145,   146,   131,   132,   133,   134,     0,   135,   136,
     137,   138,   139,   140,   311,     0,     0,     0,     0,   141,
     142,   143,   144,   145,   146,   131,   132,   133,   134,     0,
     135,   136,   137,   138,   139,   140,   312,     0,     0,     0,
       0,   141,   142,   143,   144,   145,   146,   131,   132,   133,
     134,     0,   135,   136,   137,   138,  -125,  -125,     0,     0,
       0,     0,     0,   141,   142,   143,   144,   145,   146,   132,
     133,   134,     0,   135,   136,   137,   138,   139,   140,     0,
       0,     0,     0,     0,   141,   142,   143,   144,   145,   146,
    -125,  -125,     0,  -125,  -125,  -125,  -125,  -125,  -125,     0,
       0,     0,     0,     0,   141,   142,   143,   144,   145,   146,
     133,   134,     0,   135,   136,   137,   138,   139,   140,     0,
       0,     0,     0,     0,   141,   142,   143,   144,   145,   146
};

static const yytype_int16 yycheck[] =
{
      67,    41,    35,    36,    37,     3,    95,    96,   165,     3,
       3,    29,     3,     3,   111,     0,   105,    37,    27,   108,
      89,   244,    30,   112,   113,   122,     3,    60,     4,     5,
       6,     7,    65,     9,    10,    11,    12,    13,    14,   262,
      37,    29,   111,     8,    20,    21,    22,    23,    24,    25,
      83,    84,    72,   122,   123,    72,    74,    90,     3,   282,
      37,   130,   151,   103,    73,    37,   163,   156,   272,    74,
       9,    28,    37,    12,    31,    72,   109,    75,    69,    28,
      29,    75,    75,   116,   117,    75,   175,    59,     3,   293,
     179,     3,    37,    73,   163,     3,   165,    73,   131,   132,
     133,   134,   135,   136,   137,   138,   195,     8,   141,   142,
     143,   144,   145,   146,   318,     9,    55,     3,    12,   323,
     324,   325,    37,   212,    12,    37,    14,   224,    55,    37,
       3,   164,    75,    72,   223,   168,    37,   294,    37,    85,
     229,   238,    88,   176,    76,    72,    74,   180,    74,    29,
     239,    37,    50,   193,    29,   224,    50,    55,    29,    28,
      29,    55,    31,    27,    74,   254,   255,    74,    74,   238,
      52,    53,    54,    33,    72,   244,    37,     3,    72,    31,
     213,   214,    28,    35,   273,   218,   219,     8,   221,    28,
      74,    13,    14,   262,    37,    74,   285,    37,   231,    21,
      37,   234,    55,    56,    57,    27,    28,    29,    61,    62,
      62,    31,    34,   282,    37,    35,    68,    74,    70,    71,
      74,    37,    28,   256,   291,   294,   315,   316,   317,    82,
      22,    23,    24,    25,    87,    57,    74,    33,   305,   328,
     329,    74,    62,    74,   333,   334,    74,    74,    68,   102,
      70,    71,    74,   106,   287,   322,   289,    76,    37,    25,
     327,   328,   329,    75,    78,     4,     5,     6,     7,   122,
       9,    10,    11,    12,    13,    14,    31,    74,    76,    37,
      35,    20,    21,    22,    23,    24,    25,    75,    37,    75,
       4,     5,     6,     7,   147,     9,    10,    11,    12,    13,
      14,    31,    76,    37,    37,    35,    20,    21,    22,    23,
      24,    25,     8,    68,    37,    70,    71,    27,    60,    15,
      16,    17,    18,    19,    39,    75,    79,    79,    76,   182,
      76,    76,    62,    76,     8,   188,    77,    77,    68,    78,
      70,    71,    77,    49,    29,    -1,     4,     5,     6,     7,
      -1,     9,    10,    11,    12,    13,    14,    -1,    -1,    -1,
      -1,    75,    20,    21,    22,    23,    24,    25,     4,     5,
       6,     7,    -1,     9,    10,    11,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
       4,     5,     6,     7,    -1,     9,    10,    11,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,    23,
      24,    25,    -1,     4,     5,     6,     7,    75,     9,    10,
      11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    20,
      21,    22,    23,    24,    25,     4,     5,     6,     7,    75,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,     4,     5,     6,
       7,    75,     9,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    20,    21,    22,    23,    24,    25,    -1,
       4,     5,     6,     7,    75,     9,    10,    11,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,    23,
      24,    25,     4,     5,     6,     7,    75,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    20,    21,
      22,    23,    24,    25,     4,     5,     6,     7,    75,     9,
      10,    11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    -1,     4,     5,     6,
       7,    75,     9,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    20,    21,    22,    23,    24,    25,     4,
       5,     6,     7,    75,     9,    10,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    75,     4,     5,     6,     7,
      -1,     9,    10,    11,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    75,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      75,    -1,    -1,    13,    14,    27,    28,    29,    -1,    31,
      -1,    21,    34,    35,    -1,    -1,    -1,    27,    28,    29,
      68,    31,    70,    71,    34,    35,    -1,    37,    38,    -1,
      40,    41,    42,    43,    44,    57,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    -1,    -1,    57,    58,    -1,
      -1,    -1,    74,    63,    13,    14,    -1,    -1,    68,    -1,
      70,    71,    21,    -1,    74,    -1,    -1,    77,    27,    28,
      29,    -1,    31,    -1,    -1,    34,    35,    -1,    -1,    38,
      -1,    40,    41,    42,    43,    44,    -1,    46,    47,    48,
      49,    -1,    51,    52,    53,    54,    -1,    -1,    57,    58,
      -1,    -1,    -1,    -1,    63,    13,    14,    -1,    -1,    68,
      -1,    70,    71,    21,    -1,    74,    -1,    -1,    77,    27,
      28,    29,    -1,    31,    -1,    -1,    34,    35,    -1,    -1,
      38,    -1,    40,    41,    42,    43,    44,    -1,    46,    47,
      48,    49,    -1,    51,    52,    53,    54,    -1,    -1,    57,
      58,    -1,    -1,    -1,    -1,    63,    13,    14,    -1,    -1,
      68,    -1,    70,    71,    21,    -1,    74,    -1,    -1,    77,
      27,    28,    29,    -1,    31,    -1,    -1,    34,    35,    -1,
      -1,    38,    -1,    40,    41,    42,    43,    44,    -1,    46,
      47,    48,    49,    -1,    51,    52,    53,    54,    -1,    -1,
      57,    58,    -1,    -1,    -1,    -1,    63,    13,    14,    -1,
      -1,    68,    -1,    70,    71,    21,    -1,    74,    -1,    -1,
      77,    27,    28,    29,    -1,    31,    -1,    -1,    34,    35,
      -1,    -1,    38,    -1,    40,    41,    42,    43,    44,    -1,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    -1,
      -1,    57,    58,    -1,    -1,    -1,    -1,    63,    13,    14,
      -1,    -1,    68,    -1,    70,    71,    21,    -1,    74,    -1,
      -1,    77,    27,    28,    29,    -1,    31,    -1,    -1,    34,
      35,    -1,    -1,    38,    -1,    40,    41,    42,    43,    44,
      -1,    46,    47,    48,    49,    -1,    51,    52,    53,    54,
      31,    -1,    57,    58,    35,    -1,    -1,    -1,    63,    13,
      14,    -1,    -1,    68,    -1,    70,    71,    21,    -1,    74,
      -1,    -1,    77,    27,    28,    29,    -1,    31,    -1,    -1,
      34,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    -1,
      -1,    -1,    -1,    57,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    -1,    31,    -1,    -1,    34,    35,
      74,    75,    38,    -1,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    -1,
      -1,    57,    58,    -1,    -1,    13,    14,    63,    -1,    -1,
      -1,    -1,    68,    21,    70,    71,    -1,    -1,    74,    27,
      28,    29,    -1,    31,    -1,    -1,    34,    35,    -1,    -1,
      38,    -1,    40,    41,    42,    43,    44,    -1,    46,    47,
      48,    49,    -1,    51,    52,    53,    54,    -1,    27,    57,
      58,    -1,    31,    13,    14,    63,    35,    -1,    -1,    -1,
      68,    21,    70,    71,    -1,    -1,    74,    27,    28,    29,
      -1,    31,    13,    14,    34,    35,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    62,    -1,    -1,    27,    28,    29,    68,
      31,    70,    71,    34,    -1,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    68,    31,
      70,    71,    -1,    35,    74,    -1,    57,    -1,     4,     5,
       6,     7,    -1,     9,    10,    11,    12,    13,    14,    -1,
      -1,    -1,    -1,    74,    20,    21,    22,    23,    24,    25,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,     4,     5,     6,     7,    -1,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      56,    20,    21,    22,    23,    24,    25,     4,     5,     6,
       7,    -1,     9,    10,    11,    12,    13,    14,    37,    -1,
      -1,    -1,    -1,    20,    21,    22,    23,    24,    25,     4,
       5,     6,     7,    -1,     9,    10,    11,    12,    13,    14,
      37,    -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,
      25,     4,     5,     6,     7,    -1,     9,    10,    11,    12,
      13,    14,    37,    -1,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,     4,     5,     6,     7,    -1,     9,    10,
      11,    12,    13,    14,    37,    -1,    -1,    -1,    -1,    20,
      21,    22,    23,    24,    25,     4,     5,     6,     7,    -1,
       9,    10,    11,    12,    13,    14,    37,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,     4,     5,     6,
       7,    -1,     9,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    20,    21,    22,    23,    24,    25,     5,
       6,     7,    -1,     9,    10,    11,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
       6,     7,    -1,     9,    10,    11,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
       6,     7,    -1,     9,    10,    11,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    81,    82,     0,    30,    83,    86,    28,    31,    29,
      87,    72,    84,    85,    84,    74,    27,    73,    37,    85,
      37,    28,    29,    31,   102,   103,   104,    73,    75,     3,
      37,   108,   104,    76,    88,    13,    14,    21,    27,    28,
      29,    31,    34,    35,    38,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    51,    52,    53,    54,    57,
      58,    63,    68,    70,    71,    74,    89,    90,    91,    92,
      96,    99,   100,   101,   105,   109,    28,   105,   105,   105,
       9,    12,    50,    55,    72,   106,   107,    50,   106,    74,
      74,    29,    29,    98,    98,    28,    29,    29,    95,    95,
      95,    95,    27,    31,    62,   109,    27,    62,   109,   109,
      27,    74,   105,   109,   109,   110,    74,    74,   105,    77,
      91,    37,    33,     3,     8,    15,    16,    17,    18,    19,
      97,     4,     5,     6,     7,     9,    10,    11,    12,    13,
      14,    20,    21,    22,    23,    24,    25,     8,    28,    28,
      62,   109,   105,   105,   107,    62,   109,    28,    31,    75,
     101,   105,    29,    74,    74,    74,     8,   108,     8,   108,
      37,    37,    37,    37,    62,   109,    74,   108,    62,   109,
      74,   108,   105,    37,    99,   101,   108,   108,     3,    69,
     105,   105,    75,    31,    99,   109,   101,   101,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   109,    74,     3,   108,    56,    73,    74,     3,
     108,     3,    75,    75,    74,    99,   105,    96,   101,    28,
     105,    74,   108,   105,    74,   108,   105,   109,    33,    76,
     109,    75,    75,    37,     3,   108,   108,   105,   105,   105,
     105,   105,   108,    99,    75,    75,    78,   108,   105,    75,
     105,    75,    74,    35,    99,   108,   100,    75,    37,    75,
      37,    75,    76,    75,   108,   108,   105,    75,    37,    75,
      37,   100,     3,    75,    75,    93,    37,     3,    37,     3,
      37,    90,   108,    76,    78,    37,    37,    75,   100,    59,
      94,   108,   105,   105,    77,    90,    96,    37,    75,    27,
      60,    37,    37,    39,    77,    75,    79,    79,    76,   108,
     108,   108,    90,    76,    76,    76,    77,    90,    90,    90,
      77,   108,   108,    77,    77,   108,   108,    77
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    80,    81,    82,    82,    83,    83,    84,    84,    85,
      85,    86,    86,    87,    88,    88,    89,    89,    89,    89,
      90,    90,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    92,    92,    92,    92,    92,    92,    92,    92,
      93,    93,    94,    95,    95,    96,    97,    97,    97,    97,
      97,    97,    98,    98,    99,    99,   100,   100,   101,   101,
     101,   102,   102,   103,   103,   104,   104,   104,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   106,
     106,   107,   107,   108,   108,   109,   109,   109,   109,   109,
     110,   110
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     4,     4,     2,     1,     3,
       2,     2,     0,     9,     2,     0,     3,     4,     3,     5,
       2,     1,     8,    12,     7,     6,     6,     4,     4,     3,
       3,     3,     3,     1,     1,     9,    13,     1,     4,     7,
       3,     6,     4,     7,     3,     6,     3,     8,     3,     2,
       4,    16,     7,     4,     6,     9,     4,     7,     6,     9,
       2,     0,     9,     1,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     0,     1,     3,     1,     1,
       1,     1,     0,     1,     3,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       1,     1,     3,     4,     6,     2,     8,     6,     6,     2,
       1,     3,     3,     1,     0,     2,     1,     3,     4,     4,
       1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 98 "tv.y" /* yacc.c:1646  */
    {
		prog = ALLOC(struct program);
		prog->tables = (yyvsp[-1].tables);
		prog->procs = (yyvsp[0].proc);
		prog->globals = 0;
	}
#line 1813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 108 "tv.y" /* yacc.c:1646  */
    {
		Table *s = (yyvsp[-1].tables);

		if(s == 0)
			(yyval.tables) = (yyvsp[0].tables);
		else {
			while(s->next != 0)
				s = s->next;
			s->next = (yyvsp[0].tables);
		}
	}
#line 1829 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 120 "tv.y" /* yacc.c:1646  */
    { (yyval.tables) = 0; }
#line 1835 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 125 "tv.y" /* yacc.c:1646  */
    {
		(yyval.tables) = ALLOC(Table);
		(yyval.tables)->name = (yyvsp[-2].symbol);
		(yyval.tables)->tabdim = (yyvsp[-1].tabdim);
		(yyval.tables)->mem = 0;
		(yyval.tables)->next = NULL;
	}
#line 1847 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 133 "tv.y" /* yacc.c:1646  */
    {
                (yyval.tables) = ALLOC(Table);
		(yyval.tables)->name = (yyvsp[-2].symbol);
		(yyval.tables)->tabdim = (yyvsp[-1].tabdim);
		(yyval.tables)->mem = 0;
		(yyval.tables)->next = NULL;
	}
#line 1859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 144 "tv.y" /* yacc.c:1646  */
    {
            if ((yyvsp[-1].tabdim).dimensions!=0xffffffff) {
		int *new = emalloc(((yyvsp[-1].tabdim).dimensions+1)*sizeof(int));

		memcpy(new, (yyvsp[-1].tabdim).sizes, (yyvsp[-1].tabdim).dimensions*sizeof(int));
		new[(yyvsp[-1].tabdim).dimensions] = (yyvsp[0].tabdim).sizes[0];
		free((yyvsp[-1].tabdim).sizes);
		free((yyvsp[0].tabdim).sizes);
		(yyval.tabdim).dimensions = (yyvsp[-1].tabdim).dimensions + 1;
		(yyval.tabdim).sizes = new;
            }
            else {              /* Deferred allocation */
              free((yyvsp[-1].tabdim).sizes);
              free((yyvsp[0].tabdim).sizes);
              (yyval.tabdim).dimensions = (yyvsp[-1].tabdim).dimensions + 1;
              (yyval.tabdim).sizes = NULL;
            }
	}
#line 1882 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 167 "tv.y" /* yacc.c:1646  */
    {
		(yyval.tabdim).dimensions = 1;
		(yyval.tabdim).sizes = emalloc(sizeof(int));
		(yyval.tabdim).sizes[0] = (int)((yyvsp[-1].number)+0.5);
	}
#line 1892 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 173 "tv.y" /* yacc.c:1646  */
    {
		(yyval.tabdim).dimensions = 1;
		(yyval.tabdim).sizes = emalloc(sizeof(int));
		(yyval.tabdim).sizes[0] = (int)(0xffffffff);
	}
#line 1902 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 182 "tv.y" /* yacc.c:1646  */
    {
		struct proc *p = (yyvsp[-1].proc);

		if(p == 0)
			(yyval.proc) = (yyvsp[0].proc);
		else {
			while(p->next != 0)
				p = p->next;
			p->next = (yyvsp[0].proc);
		}
		
	}
#line 1919 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 195 "tv.y" /* yacc.c:1646  */
    { (yyval.proc) = 0; }
#line 1925 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 200 "tv.y" /* yacc.c:1646  */
    {
		(yyval.proc) = ALLOC(struct proc);
		(yyval.proc)->name = (yyvsp[-8].symbol);
		(yyval.proc)->plist = (yyvsp[-6].pl);
		(yyval.proc)->locals = (yyvsp[-2].locals);
		(yyval.proc)->body = (yyvsp[-1].stat);
		(yyval.proc)->next = NULL;
		{
			int i = 0;
                        Param *pl = (yyvsp[-6].pl);
			Local *ll = (yyvsp[-2].locals);
			while(pl != 0) {
				pl = pl->next;
				i++;
			}
			(yyval.proc)->arity = i;
			i = 0;
			while(ll != 0) {
				ll = ll->next;
				i++;
			}
			(yyval.proc)->numloc = i;
		}
	}
#line 1954 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 228 "tv.y" /* yacc.c:1646  */
    {
		Local *l = (yyvsp[-1].locals);

		if(l == 0)
			(yyval.locals) = (yyvsp[0].locals);
		else {
			while(l->next != 0)
				l = l->next;
			l->next = (yyvsp[0].locals);
		}
	}
#line 1970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 240 "tv.y" /* yacc.c:1646  */
    { (yyval.locals) = 0; }
#line 1976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 245 "tv.y" /* yacc.c:1646  */
    {
		(yyval.locals) = mklocal((yyvsp[-1].symbol), value, mkexpr(NUMBER, 0, 0));
		(yyval.locals)->init->value = 0.0;
	}
#line 1985 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 250 "tv.y" /* yacc.c:1646  */
    {
		(yyval.locals) = mklocal((yyvsp[-2].symbol), value, (yyvsp[0].expr));
	}
#line 1993 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 254 "tv.y" /* yacc.c:1646  */
    {
		(yyval.locals) = mklocal((yyvsp[-1].symbol), tableptr, 0);
	}
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 258 "tv.y" /* yacc.c:1646  */
    {
		(yyval.locals) = mklocal((yyvsp[-3].symbol), tableptr, mkexpr(TIDENT, 0, 0));
		(yyval.locals)->init->sym = (yyvsp[-1].symbol);
	}
#line 2010 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 266 "tv.y" /* yacc.c:1646  */
    {
		struct statement *s = (yyvsp[-1].stat);

		while(s->next != 0)
			s = s->next;
		s->next = (yyvsp[0].stat);
	}
#line 2022 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 278 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(IF);
		(yyval.stat)->cond = (yyvsp[-5].expr);
		(yyval.stat)->body1 = (yyvsp[-1].stat);
	}
#line 2032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 284 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(ELSE);
		(yyval.stat)->cond = (yyvsp[-9].expr);
		(yyval.stat)->body1 = (yyvsp[-5].stat);
		(yyval.stat)->body2 = (yyvsp[-1].stat);
	}
#line 2043 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 291 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(CALL);
		(yyval.stat)->cond = (Expr *)(yyvsp[-5].symbol);	/* Yucc - fix this! */
		(yyval.stat)->label = (yyvsp[-4].symbol);
		(yyval.stat)->el = (yyvsp[-2].el);
	}
#line 2054 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 298 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(CALL);
		(yyval.stat)->label = (yyvsp[-4].symbol);
		(yyval.stat)->el = (yyvsp[-2].el);
	}
#line 2064 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 304 "tv.y" /* yacc.c:1646  */
    {
		Exprlist *el = ALLOC(Exprlist);
		el->e = ALLOC(Expr);
		el->e->op = STRING;
		el->e->string = (yyvsp[-3].sexpr);
		el->next = (yyvsp[-1].el);
		(yyval.stat) = mkrule((yyvsp[-4].rule), (yyvsp[-5].el), el);
	}
#line 2077 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 313 "tv.y" /* yacc.c:1646  */
    {
		Exprlist *el = ALLOC(Exprlist);
		el->e = ALLOC(Expr);
		el->e->op = STRING;
		el->e->string = (yyvsp[-1].sexpr);
		el->next = NULL;
		(yyval.stat) = mkrule((yyvsp[-2].rule), (yyvsp[-3].el), el);
	}
#line 2090 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 340 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkrule((yyvsp[-2].rule), (yyvsp[-3].el), (yyvsp[-1].el));
	}
#line 2098 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 344 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(BREAK);
		(yyval.stat)->label = (yyvsp[-1].symbol);
	}
#line 2107 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 349 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(CONTINUE);
		(yyval.stat)->label = (yyvsp[-1].symbol);
	}
#line 2116 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 354 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(LOOP);
		(yyval.stat)->label = (yyvsp[-1].symbol);
	}
#line 2125 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 359 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(RETURN);
		(yyval.stat)->label = (yyvsp[-1].symbol);
	}
#line 2134 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 364 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(END);
	}
#line 2142 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 368 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(CLS);
	}
#line 2150 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 372 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(WHILE);
		(yyval.stat)->label = (yyvsp[-7].symbol);
		(yyval.stat)->cond = (yyvsp[-5].expr);
		(yyval.stat)->body1 = (yyvsp[-1].stat);
	}
#line 2161 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 382 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(FOR);
		(yyval.stat)->label = (yyvsp[-11].symbol);
		(yyval.stat)->cond = (yyvsp[-7].expr);
		(yyval.stat)->forinit = (yyvsp[-9].stat);
		(yyval.stat)->body1 = (yyvsp[-1].stat);
		(yyval.stat)->body2 = (yyvsp[-5].stat);
	}
#line 2174 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 392 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(STOREFILE);
		(yyval.stat)->string = (yyvsp[-1].sexpr);
                (yyval.stat)->ruledataoffset = (int)((yyvsp[-2].number)+0.5);
	}
#line 2184 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 398 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(STOREFILE);
		(yyval.stat)->cond = (yyvsp[-2].expr);
                (yyval.stat)->ruledataoffset = (int)((yyvsp[-5].number)+0.5);
	}
#line 2194 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 404 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(STOREFILE);
		(yyval.stat)->string = (yyvsp[-1].sexpr);
                (yyval.stat)->ruledataoffset = 0;
	}
#line 2204 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 410 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(STOREFILE);
		(yyval.stat)->cond = (yyvsp[-2].expr);
                (yyval.stat)->ruledataoffset = 0;
	}
#line 2214 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 416 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(STORSTR);
		(yyval.stat)->string = (yyvsp[-1].sexpr);
                (yyval.stat)->ruledataoffset = (int)((yyvsp[-2].number)+0.5);
	}
#line 2224 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 422 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(STORSTR);
		(yyval.stat)->cond = (yyvsp[-2].expr);
                (yyval.stat)->ruledataoffset = (int)((yyvsp[-5].number)+0.5);
	}
#line 2234 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 428 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(STORSTR);
		(yyval.stat)->string = (yyvsp[-1].sexpr);
                (yyval.stat)->ruledataoffset = 0;
	}
#line 2244 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 434 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(STORSTR);
		(yyval.stat)->cond = (yyvsp[-2].expr);
                (yyval.stat)->ruledataoffset = 0;
	}
#line 2254 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 440 "tv.y" /* yacc.c:1646  */
    {
                (yyval.stat) = mkstat(OPSYSCALL);
                (yyval.stat)->string = (yyvsp[-1].sexpr);
	}
#line 2263 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 445 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(SEND);
		(yyval.stat)->string = (yyvsp[-6].sexpr);
                (yyval.stat)->cond = (yyvsp[-5].expr);
                (yyval.stat)->body1 = (struct statement*)(yyvsp[-4].sexpr);
		(yyval.stat)->el = (yyvsp[-2].el);
                (yyval.stat)->ruledataoffset = 0;
	}
#line 2276 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 454 "tv.y" /* yacc.c:1646  */
    {
            (yyval.stat) = mkstat(DECDIG);
            (yyval.stat)->ruledataoffset = (int)((yyvsp[-1].number)+0.5);
        }
#line 2285 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 460 "tv.y" /* yacc.c:1646  */
    {
            (yyval.stat) = mkstat('~');
            (yyval.stat)->string = (yyvsp[-1].sexpr);
            (yyval.stat)->el =(struct exprlist*)(yyvsp[-3].sexpr);
        }
#line 2295 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 466 "tv.y" /* yacc.c:1646  */
    {
            /* $2 is var; $6 the cases, $12 the default */
            (yyval.stat) = mkstat(SWITCH);
            (yyval.stat)->cond = (yyvsp[-14].expr);
            (yyval.stat)->body1 = (Statement*)(yyvsp[-10].caseind);
            (yyval.stat)->body2 = (yyvsp[-4].stat);
	}
#line 2307 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 477 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(FILLTABLEA);
		(yyval.stat)->cond = mkexpr(TIDENT, 0, 0);
		(yyval.stat)->cond->sym = (yyvsp[-6].symbol);
		(yyval.stat)->elr = ALLOC(Exprlist);
		(yyval.stat)->elr->e = (yyvsp[-2].expr);
		(yyval.stat)->elr->next = NULL;
		(yyval.stat)->el = NULL;
	}
#line 2321 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 487 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(FILLTABLE);
		(yyval.stat)->cond = mkexpr(TIDENT, 0, 0);
		(yyval.stat)->cond->sym = (yyvsp[-3].symbol);
		(yyval.stat)->string = (yyvsp[-1].sexpr);
		(yyval.stat)->el = NULL;
	}
#line 2333 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 495 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(FILLTABLE);
		(yyval.stat)->cond = mkexpr(TIDENT, 0, 0);
		(yyval.stat)->cond->sym = (yyvsp[-5].symbol);
		(yyval.stat)->string = (yyvsp[-3].sexpr);
		(yyval.stat)->el = ALLOC(Exprlist);
		(yyval.stat)->el->next = NULL;
		(yyval.stat)->el->e = (yyvsp[-1].expr);
	}
#line 2347 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 505 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(FILLTABLEA);
		(yyval.stat)->cond = mkexpr(TIDENT, 0, 0);
		(yyval.stat)->cond->sym = (yyvsp[-8].symbol);
		(yyval.stat)->elr = ALLOC(Exprlist);
		(yyval.stat)->elr->e = (yyvsp[-4].expr);
                (yyval.stat)->elr->next = NULL;
		(yyval.stat)->el = ALLOC(Exprlist);
		(yyval.stat)->el->next = NULL;
		(yyval.stat)->el->e = (yyvsp[-1].expr);
	}
#line 2363 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 517 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(FILLTABLES);
		(yyval.stat)->cond = mkexpr(SIDENT, 0, 0);
		(yyval.stat)->cond->sym = (yyvsp[-3].symbol);
		(yyval.stat)->string = (yyvsp[-1].sexpr);
		(yyval.stat)->el = NULL;
	}
#line 2375 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 525 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(FILLTABLESA);
		(yyval.stat)->cond = mkexpr(SIDENT, 0, 0);
		(yyval.stat)->cond->sym = (yyvsp[-6].symbol);
		(yyval.stat)->elr = ALLOC(Exprlist);
		(yyval.stat)->elr->e = (yyvsp[-2].expr);
		(yyval.stat)->elr->next = NULL;
		(yyval.stat)->el = NULL;
	}
#line 2389 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 535 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(FILLTABLES);
		(yyval.stat)->cond = mkexpr(SIDENT, 0, 0);
		(yyval.stat)->cond->sym = (yyvsp[-5].symbol);
		(yyval.stat)->string = (yyvsp[-3].sexpr);
		(yyval.stat)->el = ALLOC(Exprlist);
		(yyval.stat)->el->next = NULL;
		(yyval.stat)->el->e = (yyvsp[-1].expr);
	}
#line 2403 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 545 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat(FILLTABLESA);
		(yyval.stat)->cond = mkexpr(SIDENT, 0, 0);
		(yyval.stat)->cond->sym = (yyvsp[-8].symbol);
		(yyval.stat)->elr = ALLOC(Exprlist);
		(yyval.stat)->elr->e = (yyvsp[-4].expr);
                (yyval.stat)->elr->next = NULL;
		(yyval.stat)->el = ALLOC(Exprlist);
		(yyval.stat)->el->next = NULL;
		(yyval.stat)->el->e = (yyvsp[-1].expr);
	}
#line 2419 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 558 "tv.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-1].caseind)==NULL) (yyval.caseind) = (yyvsp[0].caseind);
                else {
                  Cases *s = (yyvsp[-1].caseind);
                  while (s->next!=NULL) s = s->next;
                  s->next = (yyvsp[0].caseind); (yyval.caseind) = (yyvsp[-1].caseind);
                }
          }
#line 2432 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 567 "tv.y" /* yacc.c:1646  */
    { (yyval.caseind) = NULL; }
#line 2438 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 570 "tv.y" /* yacc.c:1646  */
    {
            (yyval.caseind) = mkcases(CASE);
            (yyval.caseind)->val = (yyvsp[-7].number);
            (yyval.caseind)->body = (yyvsp[-3].stat);
            (yyval.caseind)->next = NULL;
        }
#line 2449 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 580 "tv.y" /* yacc.c:1646  */
    { (yyval.symbol) = 0; }
#line 2455 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 585 "tv.y" /* yacc.c:1646  */
    {
		(yyval.stat) = mkstat('=');
		(yyval.stat)->cond = mkexpr((yyvsp[-1].code), (yyvsp[-2].expr), (yyvsp[0].expr));	/* horrible use of cond */
	}
#line 2464 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 592 "tv.y" /* yacc.c:1646  */
    { (yyval.code) = '='; }
#line 2470 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 593 "tv.y" /* yacc.c:1646  */
    { (yyval.code) = PLUSEQ; }
#line 2476 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 594 "tv.y" /* yacc.c:1646  */
    { (yyval.code) = MINUSEQ; }
#line 2482 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 595 "tv.y" /* yacc.c:1646  */
    { (yyval.code) = TIMESEQ; }
#line 2488 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 596 "tv.y" /* yacc.c:1646  */
    { (yyval.code) = DIVEQ; }
#line 2494 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 597 "tv.y" /* yacc.c:1646  */
    { (yyval.code) = MODEQ; }
#line 2500 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 603 "tv.y" /* yacc.c:1646  */
    { (yyval.symbol) = 0; }
#line 2506 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 609 "tv.y" /* yacc.c:1646  */
    { (yyval.el) = 0; }
#line 2512 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 614 "tv.y" /* yacc.c:1646  */
    {
		(yyval.el) = ALLOC(Exprlist);
		(yyval.el)->e = (yyvsp[0].expr);
		(yyval.el)->next = NULL;
	}
#line 2522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 620 "tv.y" /* yacc.c:1646  */
    {
		Exprlist *e = (yyvsp[-2].el);
		while(e->next != 0)
			e = e->next;
		e->next = ALLOC(Exprlist);
		e->next->e = (yyvsp[0].expr);
		e->next->next = NULL;
	}
#line 2535 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 632 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr(TIDENT, 0, 0);
		(yyval.expr)->sym = (yyvsp[0].symbol);
	}
#line 2544 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 637 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr(SIDENT, 0, 0);
		(yyval.expr)->sym = (yyvsp[0].symbol);
	}
#line 2553 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 647 "tv.y" /* yacc.c:1646  */
    { (yyval.pl) = 0; }
#line 2559 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 653 "tv.y" /* yacc.c:1646  */
    {
		struct param *p = (yyvsp[-2].pl);
		while(p->next != 0)
			p = p->next;
		p->next = (yyvsp[0].pl);
	}
#line 2570 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 663 "tv.y" /* yacc.c:1646  */
    {
		(yyval.pl) = ALLOC(Param);
		(yyval.pl)->sym = (yyvsp[0].symbol);
		(yyval.pl)->next = NULL;
	}
#line 2580 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 669 "tv.y" /* yacc.c:1646  */
    {
		(yyval.pl) = ALLOC(Param);
		(yyval.pl)->sym = (yyvsp[0].symbol);
		(yyval.pl)->next = NULL;
	}
#line 2590 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 675 "tv.y" /* yacc.c:1646  */
    {
		(yyval.pl) = ALLOC(Param);
		(yyval.pl)->sym = (yyvsp[0].symbol);
		(yyval.pl)->next = NULL;
	}
#line 2600 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 684 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[-1].expr); }
#line 2606 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 686 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(ANDAND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2612 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 688 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(OROR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2618 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 690 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr('<', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 692 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr('>', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2630 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 694 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2636 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 696 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr('!', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2642 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 698 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr(EQ, mkexpr(TIDENT, 0, 0), mkexpr(TIDENT, 0, 0));
		(yyval.expr)->left->sym = (yyvsp[-2].symbol);
		(yyval.expr)->right->sym = (yyvsp[0].symbol);
	}
#line 2652 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 704 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr('!', mkexpr(TIDENT, 0, 0), mkexpr(TIDENT, 0, 0));
		(yyval.expr)->left->sym = (yyvsp[-2].symbol);
		(yyval.expr)->right->sym = (yyvsp[0].symbol);
	}
#line 2662 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 710 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(LESS_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2668 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 712 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(GTHAN_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2674 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 714 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr('+', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2680 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 716 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr('-', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2686 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 718 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr('*', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2692 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 720 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr('/', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2698 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 722 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr('%', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2704 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 724 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr('^', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2710 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 726 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(UNARY_MINUS, (yyvsp[0].expr), 0); }
#line 2716 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 728 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(PLUSPLUS, (yyvsp[0].expr), 0); }
#line 2722 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 730 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(MINUSMINUS, (yyvsp[0].expr), 0); }
#line 2728 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 732 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(PLUSPLUS+POST, (yyvsp[-1].expr), 0); }
#line 2734 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 734 "tv.y" /* yacc.c:1646  */
    { (yyval.expr) = mkexpr(MINUSMINUS+POST, (yyvsp[-1].expr), 0); }
#line 2740 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 737 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr(IDENT, 0, 0);
		(yyval.expr)->sym = (yyvsp[0].symbol);
	}
#line 2749 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 742 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr(NUMBER, 0, 0);
		(yyval.expr)->value = (yyvsp[0].number);
	}
#line 2758 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 747 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr(MFUNC, 0, 0);
		(yyval.expr)->mfunc = (yyvsp[-2].mfunc);
	}
#line 2767 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 752 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr(MFUNC, (yyvsp[-1].expr), 0);
		(yyval.expr)->mfunc = (yyvsp[-3].mfunc);
	}
#line 2776 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 757 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr(MFUNC, (yyvsp[-3].expr), (yyvsp[-1].expr));
		(yyval.expr)->mfunc = (yyvsp[-5].mfunc);
	}
#line 2785 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 762 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr('[', mkexpr(TIDENT, 0, 0), 0);
		(yyval.expr)->left->sym = (yyvsp[-1].symbol);
		(yyval.expr)->el = (yyvsp[0].el);
	}
#line 2795 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 768 "tv.y" /* yacc.c:1646  */
    {
		Exprlist *el = ALLOC(Exprlist);
		el->e = ALLOC(Expr);
		el->e->op = STRING;
		el->e->sym = lookup((yyvsp[-3].string));
		el->next = (yyvsp[-1].el);
		(yyval.expr) = mkexpr(TRY, 0, 0);
		(yyval.expr)->rule = mkrule((yyvsp[-4].rule), (yyvsp[-5].el), el);
	}
#line 2809 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 778 "tv.y" /* yacc.c:1646  */
    {
		Exprlist *el = ALLOC(Exprlist);
		el->e = ALLOC(Expr);
		el->e->op = STRING;
		el->e->sym = lookup((yyvsp[-1].string));
		el->next = NULL;
		(yyval.expr) = mkexpr(TRY, 0, 0);
		(yyval.expr)->rule = mkrule((yyvsp[-2].rule), (yyvsp[-3].el), el);
	}
#line 2823 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 788 "tv.y" /* yacc.c:1646  */
    {
		(yyval.expr) = mkexpr(TRY, 0, 0);
		(yyval.expr)->rule = mkrule((yyvsp[-2].rule), (yyvsp[-3].el), (yyvsp[-1].el));
	}
#line 2832 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 796 "tv.y" /* yacc.c:1646  */
    {
		Exprlist *e = (yyvsp[-1].el);

		while(e->next != 0)
			e = e->next;
		e->next = (yyvsp[0].el);
	}
#line 2844 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 808 "tv.y" /* yacc.c:1646  */
    {
		(yyval.el) = ALLOC(Exprlist);
		(yyval.el)->e = (yyvsp[-1].expr);
                (yyval.el)->wrap = 1;
		(yyval.el)->next = NULL;
	}
#line 2855 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 815 "tv.y" /* yacc.c:1646  */
    {
		(yyval.el) = ALLOC(Exprlist);
		(yyval.el)->e = (yyvsp[-1].expr);
		(yyval.el)->wrap = 0;
		(yyval.el)->next = NULL;
	}
#line 2866 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 830 "tv.y" /* yacc.c:1646  */
    {
		 (yyval.sexpr) = ALLOC(SEXP);
                 (yyval.sexpr)->op = '[';
                 (yyval.sexpr)->sym = (yyvsp[-1].symbol);
                 (yyval.sexpr)->el = (yyvsp[0].el);
                 (yyval.sexpr)->string = NULL;
	}
#line 2878 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 838 "tv.y" /* yacc.c:1646  */
    {
            (yyval.sexpr) = ALLOC(SEXP);
            (yyval.sexpr)->op = 0;
            (yyval.sexpr)->sym = NULL;
            (yyval.sexpr)->el = NULL;
            (yyval.sexpr)->string = (yyvsp[0].string);
        }
#line 2890 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 846 "tv.y" /* yacc.c:1646  */
    {
            (yyval.sexpr) = ALLOC(SEXP);
            (yyval.sexpr)->op = BEGCAT;
            (yyval.sexpr)->next = (yyvsp[-1].sexpr);
        }
#line 2900 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 852 "tv.y" /* yacc.c:1646  */
    {
	    	(yyval.sexpr) = ALLOC(SEXP);
            	(yyval.sexpr)->op = INT2STRING;
            	(yyval.sexpr)->el = ALLOC(Exprlist);
		(yyval.sexpr)->el->e = (yyvsp[-1].expr);
		(yyval.sexpr)->el->wrap = 0;
		(yyval.sexpr)->el->next = 0;
		(yyval.sexpr)->next = NULL;
        }
#line 2914 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 862 "tv.y" /* yacc.c:1646  */
    {
	    (yyval.sexpr) = ALLOC(SEXP);
            (yyval.sexpr)->op = NUM2STRING;
            	(yyval.sexpr)->el = ALLOC(Exprlist);
		(yyval.sexpr)->el->e = (yyvsp[-1].expr);
		(yyval.sexpr)->el->wrap = 0;
		(yyval.sexpr)->el->next = 0;
		(yyval.sexpr)->next = NULL;
        }
#line 2928 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 875 "tv.y" /* yacc.c:1646  */
    {
                (yyval.sexpr) = (yyvsp[0].sexpr);
		(yyval.sexpr)->next = NULL;
	}
#line 2937 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 880 "tv.y" /* yacc.c:1646  */
    {
		SEXP *e = (yyvsp[-2].sexpr);
		while(e->next != 0)
			e = e->next;
		e->next = (yyvsp[0].sexpr);
	}
#line 2948 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2952 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 888 "tv.y" /* yacc.c:1906  */


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
					rule->picture, rule->right_arity);
	    else if(rule->right_arity == 1)
		fprintf(stderr, "TV: rule '%s' should have one input parameter\n",
					rule->picture, rule->right_arity);
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

