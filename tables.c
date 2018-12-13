/*
 *	TABULA VIGILANS: Table Rules
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "tv.h"
#include "rules.h"
#include "xtab.h"

int
c_compare(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;
	int n;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "compare", "must have a table as output");
	    return 1;
	}
	if((n = numparam(s->elr)) != 2 && n != 3) {
	    rule_err(proc, "compare", "must have two or three inputs");
	    return 1;
	}
	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "compare", "must have tables as 1st and 2nd inputs");
	    return 1;
	}
	el = el->next;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "compare", "must have tables as 1st and 2nd inputs");
	    return 1;
	}
	el = el->next;
	if(el != 0) {
	    if(el->e->op != IDENT && el->e->op != NUMBER) {
		rule_err(proc, "compare", "3rd input parameter must be a typeflag");
		return 1;
	    }
	}
		
	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
i_compare(void **dp)
{
	*(int *)dp = 0;
	return 0;
}

int
e_compare(Statement *s, Cell *locals, void **dp)
{
/* User prototype: C compare A, B [,typeflag]
	Output - C is Output Table
	Input1 - A is 1st Comparison Table
	Input2 - B is 2nd Comparison Table
	[Input3] - Typeflag of Comparison:
		0 - Common (element appears in both tables)
		1 - Exclusive (element appears in either input table
				but not in the other)
*/
	int	typeflag, i, j, k = 0, in = 0;
	int	A_siz, B_siz, C_siz;
	int	*done = (int *)dp;
	struct	table *A, *B, *C;
	struct	expr *output = s->el->e;
	struct	exprlist *inputs  = s->elr;

	A = LGVAR(inputs->e,  locals)->u.table; inputs = inputs->next;
	B = LGVAR(inputs->e,  locals)->u.table; inputs = inputs->next;
	typeflag = (inputs != 0 ? (int)eval(inputs->e, locals) : 0);
	C = LGVAR(output, locals)->u.table;

	if((A == 0) || (B == 0)) {
	    fprintf(stderr, "TV: rule 'compare': non-existent input table!");
	    tidy_up(0);
	}
	if(C == 0) {
	    fprintf(stderr, "TV: rule 'compare': non-existent output table!");
	    tidy_up(0);
	}

	A_siz =  A->tabdim.sizes[0];
	B_siz =  B->tabdim.sizes[0];
	C_siz =  C->tabdim.sizes[0];

	if(*done == 0) /* Tables to be compared */
	{
	    for(i = 0; i < A_siz; i++) {
		if(typeflag) in = 1; else in = 0;
		for(j = 0; j < B_siz; j++) {
		    if(typeflag) {
			if(A->mem[i] != B->mem[j])
			    continue;
			in = 0;
			break;
		    }
		    else {
			if(A->mem[i] != B->mem[j])
			    continue;
			in = 1;
			break;
		    }
		}
		if(in)
		    C->mem[k++] = A->mem[i];
	    }
	    if(typeflag) {
		for(i = 0; i < B_siz; i++) {
		    in = 1;
		    for(j = 0; j < A_siz; j++) {
			if(B->mem[i] != A->mem[j])
			    continue;
			in = 0;
		    }
		    if(in) C->mem[k++] = B->mem[i];
		}
	    }
	    if(k != C_siz)
		printf("\nCOMPARE: Optimum Size of Output Table is %ld\n", k);
	    *done = 1;
	    return 1;
	}
	return 0;
}

int
c_copy_table(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "copy_table", "must have a table as output");
	    return 1;
	}

	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "copy_table", "must have a table as input");
	    return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_copy_table(Statement *s, Cell *locals, void **dp)
{
	int	i, minimum, table1_size, table2_size;
	struct	table *table1, *table2;
	struct	expr *output = s->el->e;
	struct	expr *input  = s->elr->e;

	table1 = LGVAR(output, locals)->u.table;
	table2 = LGVAR(input,  locals)->u.table;

	if(table1 == 0) {
	    fprintf(stderr, "TV: rule 'copy_table': non-existent output table!");
	    tidy_up(0);
	}
	if(table2 == 0) {
	    fprintf(stderr, "TV: rule 'copy_table': non-existent input table!");
	    tidy_up(0);
	}

	table1_size	=  table1->tabdim.sizes[0];
	table2_size	=  table2->tabdim.sizes[0];
		 /* NB - Using only 1 dimension for now */

	minimum = min(table1_size, table2_size);

	for(i = 0; i < minimum; i++)
	    table1->mem[i] = table2->mem[i];

	if(table1_size != table2_size)
		return 0;
	return 1;
}

int
c_embed(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "embed", "must have a table as output");
	    return 1;
	}

	for(el = s->elr; el != 0; el = el->next) {
	    if(el->e->op != TIDENT) {
		rule_err(proc, "embed", "must have tables as inputs");
		return 1;
	    }
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
i_embed(void **dp)
{
	*(int *)dp = 0;
	return 0;
}

int
e_embed(Statement *s, Cell *locals, void **dp)
{
	/* User prototype: C embed A, B */
	int	i, j, k, A_siz, B_siz, C_siz;
	double	mmin = HUGE_VAL;
	int	*done = (int *)dp;
	struct	table *A, *B, *C;
	struct	expr *output = s->el->e;
	struct	exprlist *inputs  = s->elr;

	C = LGVAR(output, locals)->u.table;
	A = LGVAR(inputs->e, locals)->u.table;	inputs = inputs->next;
	B = LGVAR(inputs->e, locals)->u.table;

	if((A == 0) || (B == 0)) {
	    fprintf(stderr, "TV: rule 'embed': non-existent input table!");
	    tidy_up(0);
	}
	if(C == 0) {
	    fprintf(stderr, "TV: rule 'embed': non-existent output table!");
	    tidy_up(0);
	}
	C_siz =  C->tabdim.sizes[0];
	A_siz =  A->tabdim.sizes[0];
	B_siz =  B->tabdim.sizes[0];

	if(*done == 0) {	/* Table C embedding to be done */
	    for(i = 0; i < B_siz; i++) {
		mmin = (mmin < B->mem[i] ? mmin: B->mem[i]);
	    }
	    mmin = -mmin;

	    for(i = 0, k = 0; i < A_siz; i++) {
		if(k >= C_siz) break;
		for(j = 0; j < B_siz; j++) {
		    C->mem[k++] = A->mem[i] + B->mem[j] + mmin;
		    if(k >= C_siz) break;
		}
	    }
	    *done = 1;
	    return 1;
	}
	return 0;
}

int
c_fold(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "fold", "must have a table as output");
	    return 1;
	}

	for(el = s->elr; el != 0; el = el->next) {
	    if(el->e->op != TIDENT) {
		rule_err(proc, "fold", "must have tables as inputs");
		return 1;
	    }
	}
	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
i_fold(void **dp)
{
	*(int *)dp = 0;
	return 0;
}

int
e_fold(Statement *s, Cell *locals, void **dp)
{	/* User prototype: C fold A, B */
	int	i, j, k, A_siz, B_siz, C_siz;
	double	unit = 0.0;
	int	*done = (int *)dp;
	struct	table *A, *B, *C;
	struct	expr *output = s->el->e;
	struct	exprlist *inputs  = s->elr;

	C = LGVAR(output, locals)->u.table;
	A = LGVAR(inputs->e, locals)->u.table;	inputs = inputs->next;
	B = LGVAR(inputs->e, locals)->u.table;

	if((A == 0) || (B == 0)) {
	    fprintf(stderr, "TV: rule 'fold': non-existent input table!");
	    tidy_up(0);
	}
	if(C == 0) {
	    fprintf(stderr, "TV: rule 'fold': non-existent output table!");
	    tidy_up(0);
	}
	C_siz =  C->tabdim.sizes[0];
	A_siz =  A->tabdim.sizes[0];
	B_siz =  B->tabdim.sizes[0];

	if(*done == 0) {	/* Table C folding to be done */
	    for(i = 0; i < B_siz; i++) {
		unit += B->mem[i];
	    }
	    unit = 1.0/unit;
	    for(i = 0, k = 0; i < A_siz; i++) {
		for(j = 0; j < B_siz; j++) {
		    C->mem[k++] = A->mem[i] * B->mem[j] * unit;
		}
	    }
	    *done = 1;
	    return 1;
	}
	return 0;
}

int
c_generate(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "generate", "must have a table as output");
	    return 1;
	}

	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "generate", "must have a table as input");
	    return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
i_generate(void **dp)
{
	*(int *)dp = 0;
	return 0;
}

int
e_generate(Statement *s, Cell *locals, void **dp)
{	/* User prototype: B generate A */
	int	check = 0, i, j, k, l, A_siz, B_siz, diff_siz, temp_siz;
	int	*done = (int *)dp;
	struct	table *A, *B;
	struct	expr *output = s->el->e;
	struct	expr *input  = s->elr->e;
	double  *temp_table, *diff_table;

	A = LGVAR(input,  locals)->u.table;
	B = LGVAR(output, locals)->u.table;

	if(A == 0) {
	    fprintf(stderr, "TV: rule 'generate': non-existent input table!");
	    tidy_up(0);
	}
	if(B == 0) {
	    fprintf(stderr, "TV: rule 'generate': non-existent output table!");
	    tidy_up(0);
	}

	A_siz =  A->tabdim.sizes[0];
	B_siz =  B->tabdim.sizes[0];

	if(*done == 0) /* Table B to be generated */
	{
	    temp_siz = 60; /* assume largish size for output table */
	    diff_siz = A_siz-1;
	    temp_table = (double *)emalloc(temp_siz * sizeof(double));
	    diff_table = (double *)emalloc((diff_siz) * sizeof(double));
	    for(i = 0; i < diff_siz; i++)
		diff_table[i] = fabs(A->mem[i+1] - A->mem[i]);
	    for(k = 0; k < A_siz; k++)
		temp_table[k] = A->mem[k];	/* Copy original set */
	    for(i = 0; i < A_siz; i++)
	    {
		if(k > temp_siz) { check = 1; break; }
		for(j = 0; j < diff_siz; j++) {
		    temp_table[k] = A->mem[i] + diff_table[j];
		    for(l = 0; l < k; l++)
			if(temp_table[l] == temp_table[k]) {
			    temp_table[k] = 0.0;
			    k--;
			}
		    k++;
		    temp_table[k] = A->mem[i] - diff_table[j];
		    for(l = 0; l < k; l++)
			if(temp_table[l] == temp_table[k]) {
			    temp_table[k] = 0.0;
			    k--;
			}
		    k++;
		    if(k > temp_siz) { check = 1; break; }
		}
	    }
	    if(B_siz > k) check = 1;
	    for(i = 0; i < temp_siz; i++) {
		if(temp_table[i] == 0.0) break;
		B->mem[i] = temp_table[i];
	    }
	    if(B_siz != i) check = 1;
	    if(check)
		printf("\nGENERATE: Optimum size of output table is %ld", k);
	    free(temp_table);
	    free(diff_table);
	    *done = 1;
	    return 1;
	}
	return 0;
}

int
c_interp_table(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "interp_table", "must have a table as output");
	    return 1;
	}

	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "interp_table", "must have a table as 1st input");
	    return 1;
	}
	el = el->next;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "interp_table", "must have a table as 2nd input");
	    return 1;
	}
	el = el->next;
	if(el->e->op != IDENT && el->e->op != NUMBER) {
	    rule_err(proc, "interp_table", "must have a value as 3rd input");
	    return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_interp_table(Statement *s, Cell *locals, void **dp)
{	/* Prototype: C interp_table A, B */
	int	i, minimum, min1, min2, dif;
	int	A_siz, B_siz, C_siz;
	double	interp_val;
	struct	expr *output = s->el->e;
	struct	exprlist *inputs  = s->elr;
	struct	table *A, *B, *C;

	C = LGVAR(output, locals)->u.table;
	A = LGVAR(inputs->e, locals)->u.table;	inputs = inputs->next;
	B = LGVAR(inputs->e, locals)->u.table;	inputs = inputs->next;
	interp_val = eval(inputs->e, locals);

	if((A == 0) || (B == 0)) {
	    fprintf(stderr, "TV: rule 'interp_table': non-existent input table!");
	    tidy_up(0);
	}
	if(C == 0) {
	    fprintf(stderr, "TV: rule 'interp_table': non-existent output table!");
	    tidy_up(0);
	}

	C_siz = C->tabdim.sizes[0];
	A_siz = A->tabdim.sizes[0];
	B_siz = B->tabdim.sizes[0];
		 /* Use only 1 dimension for now */

	if((A_siz == B_siz) && (A_siz == C_siz)) {
	    for(i = 0; i < C_siz; i++)
		C->mem[i] = ((1.0 - interp_val) * A->mem[i]) +
			(interp_val * B->mem[i]);
	} else {
	    min1 = min(A_siz, B_siz);
	    minimum  = min(min1, C_siz);
	    for(i = 0; i < minimum; i++)
		C->mem[i] = ((1.0 - interp_val) * A->mem[i]) +
			(interp_val * B->mem[i]);
	    if(minimum == A_siz) {
		min2 = min(B_siz, C_siz);
		dif = min2 - minimum;
		for(i = 0; i < dif; i++)
		    C->mem[minimum+i] =
			((1.0 - interp_val) * B->mem[minimum+i]);
	    }
	    if(minimum == B_siz) {
		min2 = min(A_siz, C_siz);
		dif = min2 - minimum;
		for(i = 0; i < dif; i++)
		    C->mem[minimum+i] =
			(interp_val * A->mem[minimum+i]);
	    }
	}
	return 1;
}

int
c_mult_table(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "mult_table", "must have a table as output");
	    return 1;
	}

	for(el = s->elr; el != 0; el = el->next) {
	    if(el->e->op != TIDENT) {
		rule_err(proc, "mult_table", "must have tables as inputs");
		return 1;
	    }
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_mult_table(Statement *s, Cell *locals, void **dp)
{
	int	i, minimum, min1, min2, dif,
		output_size, table1_size, table2_size;
	struct	table *output_table, *table1, *table2;
	struct	expr *output = s->el->e;
	struct	exprlist *inputs  = s->elr;

	output_table = LGVAR(output, locals)->u.table;
	table1 = LGVAR(inputs->e, locals)->u.table; inputs = inputs->next;
	table2 = LGVAR(inputs->e, locals)->u.table;

	if(output_table == 0) {
	    fprintf(stderr, "TV: rule 'mult_table': non-existent output table!");
	    tidy_up(0);
	}
	if((table1 == 0) || (table2 == 0)) {
	    fprintf(stderr, "TV: rule 'mult_table': non-existent input table!");
	    tidy_up(0);
	}

	output_size	=  output_table->tabdim.sizes[0];
	table1_size	=  table1->tabdim.sizes[0];
	table2_size	=  table2->tabdim.sizes[0];
		 /* NB - Using only 1 dimension for now */

	if((table1_size == table2_size) && (table1_size == output_size))
	{
	    for(i = 0; i < output_size; i++)
		output_table->mem[i] = (table1->mem[i] * table2->mem[i]);
	}
	else
	{
	    min1 = min(table1_size, table2_size);
	    minimum = min(min1, output_size);
	    for(i = 0; i < minimum; i++)
		output_table->mem[i] = (table1->mem[i] * table2->mem[i]);
	    if(minimum == table1_size)
	    {
		min2 = min(table2_size, output_size);
		dif = min2 - minimum;
		for(i = 0; i < dif; i++)
		    output_table->mem[minimum+i] = table2->mem[minimum+i];
	    }
	    if(minimum == table2_size)
	    {
		min2 = min(table1_size, output_size);
		dif = min2 - minimum;
		for(i = 0; i < dif; i++)
		    output_table->mem[minimum+i] = table1->mem[minimum+i];
	    }
	}
	return 1;
}

int
c_offset_table(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "offset_table", "must have a table as output");
	    return 1;
	}

	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "offset_table", "must have a table as 1st input");
	    return 1;
	}
	el = el->next;
	if(el->e->op != IDENT && el->e->op != NUMBER) {
	    rule_err(proc, "offset_table", "must have a value as 2nd input");
	    return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_offset_table(Statement *s, Cell *locals, void **dp)
{
	int	i, minimum, output_size, input_size;
	double	offset_value;
	struct	table *output_table, *input_table;
	struct	expr *output = s->el->e;
	struct	exprlist *inputs  = s->elr;

	output_table = LGVAR(output, locals)->u.table;
	input_table  = LGVAR(inputs->e, locals)->u.table; inputs = inputs->next;
	offset_value = eval(inputs->e, locals);

	if(output_table == 0) {
	    fprintf(stderr, "TV: rule 'offset_table': non-existent output table!");
	    tidy_up(0);
	}
	if(input_table == 0) {
	    fprintf(stderr, "TV: rule 'offset_table': non-existent input table!");
	    tidy_up(0);
	}
	

	output_size	=  output_table->tabdim.sizes[0];
	input_size	=   input_table->tabdim.sizes[0];
		 /* NB - Using only 1 dimension for now */

	if(output_size == input_size) 
	    for(i = 0; i < output_size; i++)
		output_table->mem[i] = (input_table->mem[i] + offset_value);
	else
	{
	    minimum = min(input_size, output_size);
	    for(i = 0; i < minimum; i++)
		output_table->mem[i] = (input_table->mem[i] + offset_value);
	}
	return 1;
}

int
c_shift(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;
	int n = numparam(s->elr);

	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "shift", "must have a table as 1st input");
	    return 1;
	}
	if(n == 2) {
	    el = el->next;
	    if(el->e->op != IDENT && el->e->op != NUMBER) {
		rule_err(proc, "shift", "must have a value as 2nd input");
		return 1;
	    }
	}
	return checkparam(s->elr, proc, prog);
}

int
e_shift(Statement *s, Cell *locals, void **dp)
{
	/* User prototype: shift A [,flag]
	        if 'flag' is non-zero, then shift is one place to left
		otherwise shift is one place to right */
	int	flag, i, A_siz;
	double	tmp;
	struct	table *A;
	struct	exprlist *inputs  = s->elr;

	A = LGVAR(inputs->e, locals)->u.table;	inputs = inputs->next;
	flag = (inputs != 0 ? (int)eval(inputs->e, locals) : 0);

	if(A == 0) {
	    fprintf(stderr, "TV: rule 'shift': non-existent input table!");
	    tidy_up(0);
	}
	A_siz =  A->tabdim.sizes[0];

	if(flag) {
	    tmp = A->mem[0];
	    for(i = 0; i < A_siz-1; i++)
		A->mem[i] = A->mem[i+1];
	    A->mem[A_siz-1] = tmp;
	}
	else {
	    tmp = A->mem[A_siz-1];
	    for(i = A_siz-2; i >= 0; i--)
		A->mem[i+1] = A->mem[i];
	    A->mem[0] = tmp;
	}
	return 1;
}

int
c_sort(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;
	int n = numparam(s->elr);

	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "sort", "must have a table as 1st input");
	    return 1;
	}
	if(n == 2) {
	    el = el->next;
	    if(el->e->op != IDENT && el->e->op != NUMBER) {
		rule_err(proc, "sort", "must have a value as 2nd input");
		return 1;
	    }
	}
	return checkparam(s->elr, proc, prog);
}

static int
dcmp(const void *a, const void *b)
{
	double aa = *(double *)a;
	double bb = *(double *)b;

	if(aa < bb)
		return -1;
	if(aa == bb)
		return 0;
	return 1;
}

int
e_sort(Statement *s, Cell *locals, void **dp)
{
	int	i, j, n, sort_flag;
	double  *temp_table;
	struct	table	 *input_table;
	struct	exprlist *inputs  = s->elr;

	input_table  = LGVAR(inputs->e, locals)->u.table;
	if(input_table == 0) {
	    fprintf(stderr, "TV: rule 'sort': input table does not exist!");
	    tidy_up(0);
	}
	inputs = inputs->next;
	if(inputs)
	    sort_flag = (int)eval(inputs->e, locals);
		  /* sort_flag will be 0 for increasing sort,
			anything else (1) for decreasing */

	n =   input_table->tabdim.sizes[0];
		 /* NB - Using only 1 dimension for now */

	temp_table = (double *)emalloc(n * sizeof(double));
	for(i = 0; i < n; i++)
	    temp_table[i] = input_table->mem[i];

	qsort(temp_table, n, sizeof(double), dcmp);

	if(sort_flag)
	    for(i = 0, j = n-1; i < n; i++, j--)
		input_table->mem[i] = temp_table[j];
	else
	    for(i = 0; i < n; i++)
		input_table->mem[i] = temp_table[i];
	free(temp_table);

	return 1;
}

int
c_scale_table(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "scale_table", "must have a table as output");
	    return 1;
	}

	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "scale_table", "must have a table as 1st input");
	    return 1;
	}
	el = el->next;
	if(el->e->op != IDENT && el->e->op != NUMBER) {
	    rule_err(proc, "scale_table", "must have a value as 2nd input");
	    return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_scale_table(Statement *s, Cell *locals, void **dp)
{
	int	i, minimum, output_size, input_size;
	double	scale_value;
	struct	table *output_table, *input_table;
	struct	expr *output = s->el->e;
	struct	exprlist *inputs  = s->elr;

	output_table = LGVAR(output, locals)->u.table;
	input_table  = LGVAR(inputs->e, locals)->u.table; inputs = inputs->next;
	scale_value = eval(inputs->e, locals);

	if(output_table == 0) {
	    fprintf(stderr, "TV: rule 'scale_table': non-existent output table!");
	    tidy_up(0);
	}
	if(input_table == 0) {
	    fprintf(stderr, "TV: rule 'scale_table': non-existent input table!");
	    tidy_up(0);
	}
	output_size	=  output_table->tabdim.sizes[0];
	input_size	=   input_table->tabdim.sizes[0];
		 /* NB - Using only 1 dimension for now */

	if(output_size == input_size) 
	    for(i = 0; i < output_size; i++)
		output_table->mem[i] = (input_table->mem[i] * scale_value);
	else
	{
	    minimum = min(input_size, output_size);
	    for(i = 0; i < minimum; i++)
		output_table->mem[i] = (input_table->mem[i] * scale_value);
	}
	return 1;
}

int
c_subst(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "subst", "must have a table as output");
	    return 1;
	}

	el = s->elr;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "subst", "must have a table as 1st input");
	    return 1;
	}
	el = el->next;
	if(el->e->op != TIDENT) {
	    rule_err(proc, "subst", "must have a table as 2nd input");
	    return 1;
	}
	el = el->next;
	if(el->e->op != IDENT && el->e->op != NUMBER) {
	    rule_err(proc, "subst", "must have a value as 3rd input");
	    return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_subst(Statement *s, Cell *locals, void **dp)
{
/* 	Prototype: C subst A, B, fac
	means:	when fac = 0, C copies A
		when fac = 1, C copies B
		in between, each value of C is drawn from A or B
		depending on statictical substitution index fac
*/
	int	i, minimum, A_siz, B_siz, C_siz;
	double	fac, r;
	struct	table *A, *B, *C;
	struct	expr *output = s->el->e;
	struct	exprlist *inputs  = s->elr;

	C = LGVAR(output, locals)->u.table;
	A = LGVAR(inputs->e, locals)->u.table; inputs = inputs->next;
	B = LGVAR(inputs->e, locals)->u.table; inputs = inputs->next;

	if((A == 0) || (B == 0)) {
	    fprintf(stderr, "TV: rule 'subst': non-existent input table!");
	    tidy_up(0);
	}
	if(C == 0) {
	    fprintf(stderr, "TV: rule 'subst': non-existent output table!");
	    tidy_up(0);
	}

	fac = fabs(eval(inputs->e, locals));
	if(fac > 1.0) fac = 1.0/fac;	/* Force fac to lie between 0 & 1 */

	A_siz	=  A->tabdim.sizes[0];
	B_siz	=  B->tabdim.sizes[0];
	C_siz	=  C->tabdim.sizes[0];
		 /* NB - Using only 1 dimension for now */

	minimum = min(A_siz, B_siz);
	minimum = min(minimum, C_siz);
	for(i = 0; i < minimum; i++) {
		r = drand48();
		if(r > fac)
		    C->mem[i] = A->mem[i];
		else
		    C->mem[i] = B->mem[i];
	}
	return 1;
}

int
c_sum_table(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;

	if(s->el->e->op != TIDENT) {
	    rule_err(proc, "sum_table", "must have a table as output");
	    return 1;
	}

	for(el = s->elr; el != 0; el = el->next) {
	    if(el->e->op != TIDENT) {
		rule_err(proc, "sum_table", "must have tables as inputs");
		return 1;
	    }
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_sum_table(Statement *s, Cell *locals, void **dp)
{
	int	i, minimum, minimum1, minimum2, dif,
		output_size, table1_size, table2_size;
	struct	table *output_table, *table1, *table2;
	struct	expr *output = s->el->e;
	struct	exprlist *inputs  = s->elr;

	output_table = LGVAR(output, locals)->u.table;
	table1 = LGVAR(inputs->e, locals)->u.table; inputs = inputs->next;
	table2 = LGVAR(inputs->e, locals)->u.table;

	if(output_table == 0) {
	    fprintf(stderr, "TV: rule 'sum_table': non-existent output table!");
	    tidy_up(0);
	}
	if((table1 == 0) || (table2 == 0)) {
	    fprintf(stderr, "TV: rule 'ssum_table': non-existent input table!");
	    tidy_up(0);
	}
	output_size	=  output_table->tabdim.sizes[0];
	table1_size	=  table1->tabdim.sizes[0];
	table2_size	=  table2->tabdim.sizes[0];
		 /* NB - Using only 1 dimension for now */

	if((table1_size == table2_size) && (table1_size == output_size))
	{
	    for(i = 0; i < output_size; i++)
		output_table->mem[i] = (table1->mem[i] + table2->mem[i]);
	}
	else
	{
	    minimum1 = min(table1_size, table2_size);
	    minimum = min(minimum1, output_size);
	    for(i = 0; i < minimum; i++)
		output_table->mem[i] = (table1->mem[i] + table2->mem[i]);

	    if(minimum == table1_size)
	    {
		minimum2 = min(table2_size, output_size);
		dif = minimum2 - minimum;
		for(i = 0; i < dif; i++)
		    output_table->mem[minimum+i] = table2->mem[minimum+i];
	    }

	    if(minimum == table2_size)
	    {
		minimum2 = min(table1_size, output_size);
		dif = minimum2 - minimum;
		for(i = 0; i < dif; i++)
		    output_table->mem[minimum+i] = table1->mem[minimum+i];
	    }
	}
	return 1;
}

int
c_xadr(Statement *s, Proc *proc, Program *prog)
{
	if(s->el->e->op != TIDENT) {
	    rule_err(proc, s->rule->picture, "must have a table as output");
	    return 1;
	}

	if(s->elr->e->op != TIDENT) {
	    rule_err(proc, s->rule->picture, "must have a table as input");
	    return 1;
	}
		
	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
i_xadr(void **dp)
{
	*(int *)dp = 0;
	return 0;
}

int
e_xadr(Statement *s, Cell *locals, void **dp)
{
	/* User prototype: B xad A - eXtract Adjacent Differences
		or	 : B xar A - eXtract Adjacent Ratios
		Output - B is Output Table
		Input  - A is Input Table
    The function extracts differences (or ratios) between adjacent values in
    the input table, and writes them to the output table, optimally wrapping
    beginning to end.
*/
	int	i, mode;
	int	A_siz, B_siz, min_siz;
	int	*done = (int *)dp;
	struct	table *A, *B;
	struct	expr *output = s->el->e;
	struct	expr *input  = s->elr->e;

	A = LGVAR(input,  locals)->u.table;
	B = LGVAR(output, locals)->u.table;

	if(A == 0) {
	    if(strcmp(s->rule->picture, "xad") == 0)
		fprintf(stderr, "TV: rule 'xad': non-existent input table!");
	    else
		fprintf(stderr, "TV: rule 'xar': non-existent input table!");
	    tidy_up(0);
	}
	if(B == 0) {
	    if(strcmp(s->rule->picture, "xad") == 0)
		fprintf(stderr, "TV: rule 'xad': non-existent output table!");
	    else
		fprintf(stderr, "TV: rule 'xar': non-existent output table!");
	    tidy_up(0);
	}

	A_siz =  A->tabdim.sizes[0];
	B_siz =  B->tabdim.sizes[0];

	if(*done == 0) { 	/* To be done */
	    if(strcmp(s->rule->picture, "xad") == 0)
		mode = 0;
	    else
		mode = 1;
	    min_siz = min(A_siz, B_siz);
	    if(mode) {
		for(i = 0; i < A_siz; i++)
		    B->mem[i] = A->mem[(i+1)%min_siz] / A->mem[i];
	    }
	    else {
		for(i = 0; i < A_siz; i++)
		    B->mem[i] = A->mem[(i+1)%min_siz] - A->mem[i];
	    }
	    *done = 1;
	}
	return 1;
}
