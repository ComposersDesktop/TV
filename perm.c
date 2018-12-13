/*
 *	TABULA VIGILANS: Permutation functions
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "tv.h"
#include "rules.h"
#include "xtab.h"

#define	MAXSZ	50

static int	array[MAXSZ];

int	A_siz, factsize, permstep;
double	*primo;
struct	table *A;

static void
swop(short i,short j)
{
	short	t;

	t = array[i-1];
	array[i-1] = array[j-1];
	array[j-1] = t;
}

void perm(long pn)
{
	short	i, t, cnode;
	static	short	posa[MAXSZ+1];	/* set to zero by default */
	short		post[MAXSZ+1];

	if(pn==0) {			/* Reset posa[] if pn zero */
		for(i=0; i<MAXSZ; i++)
			posa[i] = 0;
		return;
	}

	post[0] = 0;			/* set up offset position array */
	for(i=1; i<A_siz; i++) {
		post[i] = pn % (i+1);
		pn /= (i+1);
	}

	for(i=1; i<A_siz; i++) {
		t = posa[i]+post[i];
		post[i] = t % (i+1);
		post[i+1] += (t / (i+1));
	}

	for(i=1; i<A_siz; i++)		/* find lowest common node */
		if(posa[i] != post[i])
			break;
	cnode = i-1;
					/* Reverse up tree to common node */
	for(i=A_siz-1; i>cnode; i--) {
		swop((short)(i+1),(short)(i-posa[i]+1));
	}
					/* Advance down tree to new elememt */
	for(i=cnode+1; i<A_siz; i++) {
		swop((short)(i+1), (short)(i-post[i]+1));
	}
					/* update position array */
	for(i=1; i<A_siz; i++)
		posa[i] = post[i];
}

long
fact(short n)		/* factorial function */
{
	long	a;
	if(n < 2)
		return 1;
	a = 2;
	for( ; n > 2; n--)
		a = a*n;
	return a;
}

void
swap(Table *tab, int i, int j)
{
    double t = tab->mem[i];
    tab->mem[i] = tab->mem[j];
    tab->mem[j] = t;
}

void
rperm(Table *tab, long tabsize)
{
    int i;

    for(i = tabsize-1; i > 1; i--)
	swap(tab, (int)(drand48() * i), i);
}

int
c_perm(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;
	int	k;

	/* Set up inital array of indices 1 .... n) */
	for(k=0; k<MAXSZ; k++)
		array[k] = k+1;

	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "perm", "must have a table as 1st input");
	    return 1;
	}
	return checkparam(s->elr, proc, prog);
}

int
e_perm(Statement *s, Cell *locals, void **dp)
{
/* User prototype: perm A
		   OR
		   perm A, step
	Input1 - A is a Table whose contents are to be randomly permuted
	Input2 - step permutation size
*/
	int	i;

	struct	exprlist *inputs  = s->elr;

	A = LGVAR(inputs->e,  locals)->u.table;
	if(A == 0) {
	    fprintf(stderr, "TV: rule 'perm': non-existent input table!");
	    tidy_up(0);
	}
	A_siz =  A->tabdim.sizes[0];
	if(A_siz > MAXSZ) {
	    fprintf(stderr, "TV: rule 'perm': maximum size of tables for permutations is 50!");
	    tidy_up(0);
	}
	factsize = fact((short)A_siz);

	if(primo == 0) {
		primo = emalloc(A_siz * sizeof(double));
		for(i=0; i<A_siz; i++)
			primo[i] = A->mem[i];
	}
	inputs = inputs->next;
	if(inputs == 0)
		rperm(A, A_siz);
	else {
	    permstep = (int) eval(inputs->e, locals);
	    while(permstep < 0)
		permstep += factsize;
	    permstep %= factsize;
	    perm(permstep);
	    for(i=0; i<A_siz; i++) {
		A->mem[i] = primo[array[i]-1];
	    }
	}
	return 1;
}

