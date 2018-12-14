/*
 *	New tv header file
 */
#include <fcntl.h>

#define MAXSTRING 161
#define	TRUE	1
#define FALSE	0

int	yylex(void);
void	gem_init(void);
void	inits(int dev_midiin,int dev_midiout);
void	perf_time(int);		/* stores time of performance */
void	tidy_up(int);
void	mach_cleanup(void);	/* machine-dependent cleanup */

enum celltype {
	value,
	tableptr,
	tableglobal,
	ruledata
};

typedef struct cell {
	enum celltype type;
	char    lock;
	char	rulefree;
	union {
		double d;
		struct table *table;
		void *ruledata;
	} u;
} Cell;

typedef struct program {
	struct table *tables;
	struct proc *procs;
	struct global *globals;
} Program;

typedef struct global {
	struct symbol *name;
	struct cell value;
	struct global *next;
} Global;

struct tabdim {
	int dimensions;
	int *sizes;		/* size in this dimension */
	int *spaces;		/* space required in this dimension */
};

typedef struct table {
	struct symbol *name;
	struct tabdim tabdim;
	double *mem;
	struct table *next;
} Table;

typedef struct tables {
	struct symbol *name;
	struct tabdim tabdim;
	char   *mem;
	struct table *next;
} TableS;

typedef struct proc {
	struct symbol *name;
	int arity;
	struct param *plist;
	int numloc;
	struct local *locals;
	struct statement *body;
	struct proc *next;
} Proc;

typedef struct param {
	struct symbol *sym;
	struct param *next;
} Param;

typedef struct local {
	struct symbol *name;
	enum celltype type;
	struct expr *init;
	struct rule *rule;
	struct local *next;
} Local;

typedef struct statement {
	int code;
	struct rule *rule;
	int ruledataoffset;
	struct expr *cond;
	struct statement *forinit;
	struct statement *body1;
	struct statement *body2;
	struct symbol *label;
	struct proc *proc;
	struct exprlist *el;
	struct exprlist *elr;
	struct sexpr *string;
	struct statement *next;
} Statement;

typedef struct cases {
	int code;
        double val;
        struct statement *body;
	struct cases *next;
} Cases;

typedef struct exprlist {
	struct expr *e;
	int wrap;
	struct exprlist *next;
} Exprlist;

typedef struct expr {
	int op;
	struct mfunc *mfunc;
	struct symbol *sym;
	struct sexpr *string;
	int indx;		/* posn of symbol in params, or -1 for global */
	struct global *glob;	/* or pointer to global */
	double value;
	struct expr   *left;
	struct expr   *right;
	struct statement *rule;
	struct exprlist *el;
} Expr;

typedef struct sexpr {
	int op;
        struct symbol *sym;
	struct exprlist *el;
	char *string;
        struct sexpr *next;
} SEXP;

struct mfunc {
	char *picture;
	int arity;
	int fptab;
#ifdef LATTICE
	double (*exec)();
#else
	double (*exec)(/*...*/);
#endif
};

typedef struct rule {
	char *picture;
	int total_arity;
	int left_arity;
	int right_arity;
	int string;
	int (*exec)(struct statement *, struct cell *, void **);
	int (*init)(void **);
	int (*check)(struct statement *, struct proc *, struct program *);
} Rule;

struct keytab {
	char *picture;
	int code;
	struct rule *rule;
	struct mfunc *mfunc;
};
extern struct keytab keytab[];

typedef struct symbol {
	char *picture;
	int  keywcode;
	struct rule *rule;
	struct mfunc *mfunc;
	struct symbol *next;
} Symbol;

typedef struct sfile {
  int		fnum;		/* Indicate which storefile */
  char		*store_buf;
  char		*storefile;	/* Name of text storage file */
  int		storefd;	/* Storage file descriptor */
  struct sfile*	next;
} Sfile;

typedef struct FILES {
  FILE*	fd;
  char  *name;
  int   linenum;
  int   encrypt;                /* Is this file encryped */
  int   charp;                  /* Save current position on switch */
  long  data[2];
  struct FILES *next;
} FILES;

extern FILES *infiles;
extern int linenum;

void initsymtab(void);
struct symbol *lookup(char *);

void *emalloc(unsigned int);
#define ALLOC(s)	((s *)emalloc(sizeof(s)))

int istablesym(struct symbol *);
int isstablesym(struct symbol *);

void execute(Program *prog);
int pass2(Program *prog);
int checkexpr(Expr *, Proc *, Program *);
int checkstring(SEXP *, Proc *, Program *);
int numparam(Exprlist *);
int checkparam(Exprlist *, Proc *, Program *);
double eval(Expr *, struct cell *);
double *dereftab(Table *tab, Exprlist *idxs, struct cell *params);
struct expr *getparam(Exprlist *el, int n);

void prprog(Program *prog, FILE *f);

extern struct rule r_copy;
extern int finish;

extern int prog_argc;
extern char **prog_argv;

#define POST	(10000)
/*
 *	local/global access
 */
#define LGVAR(c,l)	(((c)->indx < 0) ? &(c)->glob->value : &(l)[(c)->indx])

/*
 *	Debugging support options
 */
extern int dump_globals;

/*
 *	extra stuff for portability
 */
#ifndef min
#define min(a,b)	(((a)<(b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)	(((a)>(b)) ? (a) : (b))
#endif

#ifdef _WIN32
double
drand48(void);
#endif
void list_mididevs(void);

extern int yyparse();
extern void yyerror(char *str);
extern int checkident(Expr *e, Proc *proc, Program *prog);
