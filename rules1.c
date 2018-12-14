/*
 *	TABULA VIGILANS: Rule functions, Set 1
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "tv.h"
#include "rules.h"
#include "xtab.h"
extern char *sexp(SEXP*, Cell *);

void
rule_err(Proc *prog, char *rule, char *msg)
{
	fprintf(stderr, "TV: rule '%s' in procedure '%s': %s\n", rule,
				prog->name->picture, msg);
}

int
c_add_dec(Statement *s, Proc *proc, Program *prog)
{
	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_add_dec(Statement *s, Cell *locals, void **dp)
{
/*
	Prototype:
		out add_dec comp1, comp2, value
*/
	double out, comp1, comp2, value;
	Expr *output = s->el->e;
	Exprlist *inputs = s->elr;

	out   = eval(output, locals);
	comp1 = eval(inputs->e, locals);	inputs = inputs->next;
	comp2 = eval(inputs->e, locals);	inputs = inputs->next;
	value = eval(inputs->e, locals);

	if(comp1 != comp2) {
		LGVAR(output, locals)->u.d =
		comp1 < comp2 ? out+value : out-value;
		return 1;
	}
	else {
		return 0;
	}
}

int
e_alllocked(Statement *s, Cell *locals, void **dp)
{
	int rc = 1;
	Exprlist *e = s->elr;

	while(e != 0) {
		if(!LGVAR(e->e,locals)->lock)
			rc = 0;
		e = e->next;
	}
	return rc;
}

int
e_anylocked(Statement *s, Cell *locals, void **dp)
{
	int rc = 0;
	Exprlist *e = s->elr;

	while(e != 0) {
		if(LGVAR(e->e,locals)->lock)
			rc = 1;
		e = e->next;
	}
	return rc;
}

int
c_copy(Statement *s, Proc *proc, Program *prog)
{
	Exprlist *el;
	int errs = 0;
	int istable = 0;

	for(el = s->el; el != 0; el = el->next) {
		if((errs += checkexpr(el->e, proc, prog)) == 0) {
			switch(el->e->op) {
			case IDENT:
				break;
			case TIDENT:
				istable++;
				break;
			case '[':
				break;
			default:
				rule_err(proc, "copy",
				    "must be identifier on left of copy rule");
				errs++;
			}
		}
	}
	if(istable && s->elr->e->op != TIDENT) {
		rule_err(proc, "copy", "all parameters must be same type");
		errs++;
	}
	return errs + checkexpr(s->elr->e, proc, prog);
}

int
e_copy(Statement *s, Cell *locals, void **dp)
{
	Exprlist *dests = s->el;
	Expr *src  = s->elr->e;
	double val;

	if(src->op == TIDENT) {
		Table *tab = LGVAR(src,locals)->u.table;
		for(; dests != 0; dests = dests->next)
			LGVAR(dests->e,locals)->u.table = tab;
		return 1;
	}

	val = eval(src, locals);

	for(; dests != 0; dests = dests->next) {
		if(dests->e->op == '[')
			*dereftab(LGVAR(dests->e,locals)->u.table, dests->e->el, locals)
			= val;
		else {
			Cell *cell = LGVAR(dests->e, locals);
			if(cell->lock)
				return 0;
			cell->u.d = val;
		}
	}
	return 1;
}

int
c_fail(Statement *s, Proc *proc, Program *prog)
{
	return 0;
}

int
e_fail(Statement *s, Cell *locals, void **dp)
{
	return 0;
}

int
c_lim(Statement *s, Proc *proc, Program *prog)
{
	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_lim(Statement *s, Cell *locals, void **dp)
{
/*
	Output Cell(s):	 Current Output Value;
	Input Cell 1 (Required): Lower bound for cell value
	Input Cell 2 (Required): Upper bound for cell value
*/
	double low, high, swap;
	Exprlist *outputs;
	Exprlist *inputs = s->elr;

	low = eval(inputs->e, locals);
	inputs = inputs->next;
	high = eval(inputs->e, locals);

	if(high < low) {
	    swap = high;
	    high = low;
	    low	 = swap;
	}

	for(outputs = s->el; outputs != 0; outputs = outputs->next) {
		if(LGVAR(outputs->e,locals)->lock)
			return -1; /* output cell locked */
		if(eval(outputs->e, locals) < low) {
			LGVAR(outputs->e, locals)->u.d = low;
			return 1; /* limiting has been done */
		}
		if(eval(outputs->e, locals) > high) {
			LGVAR(outputs->e, locals)->u.d = high;
			return 1; /* limiting has been done */
		}
	}

	return 0; /* nothing has been done */
}

struct linexplogdata {
	int direction;
	int first;
	int target_reached;
	int num_inputs;
	TICK starttime;
	TICK targettime;
	TICK dur;
};

int
c_linexplog(Statement *s, Proc *proc, Program *prog)
{
	int i;

	if(numparam(s->el) != 1) {
		rule_err(proc, s->rule->picture, "must have one output cell");
		return 1;
	}
	if((i = numparam(s->elr)) != 1 && i != 2 && i != 4) {
		rule_err(proc, s->rule->picture,
			"must have one, two or four input cells");
		return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
i_linexplog(void **dp)
{
	struct linexplogdata *ld;

	*dp = emalloc(sizeof(struct linexplogdata));
	ld = (struct linexplogdata *)*dp;

	ld->direction = 0;
	ld->first = 1;
	ld->target_reached = 1;
	return 1;
}

int
e_linexplog(Statement *s, Cell *locals, void **dp)
{
/*
	Output Cell:  Current Output Value;
	Input Cell 1 (Required): User-supplied segment duration
	Input Cell 2 (Optional): direction flag - 0 start from 0 (default)
						  1 start from 1
	Input Cell 3 (Optional): scale value 1
	Input Cell 4 (Optional): scale value 2
*/
	int mode;	/* 0 = 'lin'; 1 = 'exp'; 2 = 'log' */
	double comp, dur, scale1, scale2, diff;
	Exprlist *inputs = s->elr;
	Expr *out  = s->el->e;
	unsigned long timevalue = current_time();
	struct linexplogdata *ld = *(struct linexplogdata **)dp;

	if(strcmp(s->rule->picture, "lin") == 0)
		mode = 0;				/* 'lin' */
	else if(strcmp(s->rule->picture, "exp") == 0)
		mode = 1;				/* 'exp' */
	else
		mode = 2;				/* 'log' */
	ld->num_inputs = numparam(s->elr);
	if(ld->target_reached) {
		ld->starttime = timevalue;
		dur = eval(inputs->e, locals);
		inputs = inputs->next;
		if(ld->first) {
			if(inputs != 0)
				ld->direction = (int)eval(inputs->e, locals);
			ld->first = 0;
		}
		ld->targettime = timevalue + (unsigned long)(dur * TICKSPERSEC);
		ld->dur = (unsigned long)(dur * TICKSPERSEC);		/* Dur as long */
		ld->target_reached = FALSE;
	}

	if(ld->num_inputs == 4) {
		inputs = s->elr->next;
		inputs = inputs->next;
		scale1 = eval(inputs->e, locals);
		inputs = inputs->next;
		scale2 = eval(inputs->e, locals);
		if(scale2 < scale1) {
			diff = scale2;
			scale2 = scale1;
			scale1 = diff;
		}
		diff = scale2 - scale1;
	}
	else {
		scale1 = 0.0;
		diff = 1.0;
	}
	if(ld->direction)
		comp = (1.0 - (double)(timevalue - ld->starttime) / ld->dur);
	else
		comp = (double)(timevalue - ld->starttime) / ld->dur;

	if(mode == 1)
		comp = (-log10(1.0 - comp)/2);
	if(mode == 2)
		comp = 1.0 - (-log10(comp)/2);
	comp = (comp > 1.0 ? 1.0 : comp);
	comp = (comp < 0.0 ? 0.0 : comp);

	LGVAR(out, locals)->u.d = comp * diff + scale1;

	if(timevalue >= ld->targettime) {
		ld->direction = !ld->direction;
		ld->target_reached = TRUE; /* ready for next segment */
	}
	return (ld->target_reached);
}

int
c_lintrans(Statement *s, Proc *proc, Program *prog)
{
	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_lintrans(Statement *s, Cell *locals, void **dp)
{
/*
	Combined multiply and add, implements linear transformation as:
		a = b*c + d
	Output Cell:  as in 'a' above
	Input Cell 1: as in 'b' above
	Input Cell 2: as in 'c' above
	Input Cell 3: as in 'd' above
	NB if 'a' is identical to 'b', result will be incremental
*/
	double in, mult, add;
	Expr *output = s->el->e;
	Exprlist *inputs = s->elr;

	in = eval(inputs->e, locals);	inputs = inputs->next;
	mult = eval(inputs->e, locals);	inputs = inputs->next;
	add = eval(inputs->e, locals);

	if(LGVAR(output,locals)->lock)
		return 0;

	LGVAR(output, locals)->u.d = in * mult + add;
	return 1;
}

int
c_lock(Statement *s, Proc *proc, Program *prog)
{
	int rc = 0;
	Exprlist *e = s->elr;

	if(numparam(e) < 1) {
		rule_err(proc, s->rule->picture, "must have at least one input");
		return 1;
	}

	while(e != 0) {
		switch(e->e->op) {
		case IDENT:
			rc += checkident(e->e, proc, prog);
			break;
		case TIDENT:
			fprintf(stderr, "Can't %s a table variable\n", s->rule->picture);
			rc++;
			break;
		default:
			fprintf(stderr, "Can't %s an expression\n", s->rule->picture);
			rc++;
			break;
		}
		e = e->next;
	}
	return rc;
}

int
e_lock(Statement *s, Cell *locals, void **dp)
{
	Exprlist *e = s->elr;

	while(e != 0) {
		LGVAR(e->e,locals)->lock = 1;
		e = e->next;
	}
	return 1;
}

int
c_max(Statement *s, Proc *proc, Program *prog)
{
	if(numparam(s->elr) < 1) {
		rule_err(proc, "max", "must have input cells");
		return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_max(Statement *s, Cell *locals, void **dp)
{
/*
	Output Cell:  Current maximum value
	Input Cells (any number): cells to monitor for maximum value
*/
	Expr *output = s->el->e;
	Exprlist *inputs = s->elr;

	if(LGVAR(output,locals)->lock)
	    return 0;

	if(eval(output, locals) == 0)
	    LGVAR(output, locals)->u.d = -HUGE_VAL;

	for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
	    if(eval(output, locals) < eval(inputs->e, locals))
		LGVAR(output, locals)->u.d = eval(inputs->e, locals);
	}
	return 1;
}

int
c_mean(Statement *s, Proc *proc, Program *prog)
{
	if(numparam(s->elr) < 1) {
		rule_err(proc, "mean", "must have input cells");
		return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_mean(Statement *s, Cell *locals, void **dp)
{
/*
	Output Cell:  Current mean value
	Input Cells (any number): cells to monitor for mean value
*/
	Expr *output = s->el->e;
	Exprlist *inputs = s->elr;
	double total = 0.0;

	if(LGVAR(output,locals)->lock)
	    return 0;

	for(inputs = s->elr; inputs != 0; inputs = inputs->next)
	    total += eval(inputs->e, locals);

	LGVAR(output, locals)->u.d = total / numparam(s->elr);

	return 1;
}

int checkstring(SEXP* ss, Proc *proc, Program *prog)
{
	if (ss && ss->op == '[')
		return checkparam(ss->el, proc, prog);
	if (ss && ss->op == BEGCAT) {
		int errs = 0;
		while ((ss = ss->next)) {
			errs+=checkstring(ss, proc, prog);
		}
		return errs;
	}
	if (ss && (ss->op == INT2STRING || ss->op == NUM2STRING))
		return checkexpr(ss->el->e, proc, prog);
	return 0;		/* raw string constant */
}

int
c_message(Statement *s, Proc *proc, Program *prog)
{
	Exprlist * ee = s->elr;
	SEXP *ss = ee->e->string;
	int errs = checkstring(ss, proc, prog);
	errs += checkparam(ee, proc, prog);
	return errs;
}

int
i_message(void **dp)
{
	*(TICK *)dp = 0;
	return 0;
}

int e_message(Statement *s, Cell *locals, void **dp)
{
	int	*once = (int *)dp;
	// Finalise stringexr to symbol
	//printf("e_message: locals = %p\n", locals);
	char * ss = sexp(s->elr->e->string, locals);

	if(strcmp(s->rule->picture, "messag1") == 0) {
		if(*once)
			return 0;
		else {
			(void)fputs(ss, stdout);
			*(int *)dp = 1;
		}
	}
	else
		(void)fputs(ss, stdout);
	return 1;
}

int e_messageq(Statement *s, Cell *locals, void **dp)
{
	char *ss = sexp(s->elr->e->string,locals);
	putc('"',stdout);
	(void)fputs(ss, stdout);
	putc('"',stdout);
	return 1;
}

int
c_min(Statement *s, Proc *proc, Program *prog)
{
	if(numparam(s->elr) < 1) {
		rule_err(proc, "min", "must have input cells");
		return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_min(Statement *s, Cell *locals, void **dp)
{
/*
	Output Cell:  Current minimum value
	Input Cells (any number): cells to monitor for minimum value
*/
	Expr *output = s->el->e;
	Exprlist *inputs = s->elr;

	if(LGVAR(output,locals)->lock)
	    return 0;

	if(eval(output, locals) == 0)
	    LGVAR(output, locals)->u.d = HUGE_VAL;

	for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
	    if(eval(output, locals) > eval(inputs->e, locals))
			LGVAR(output, locals)->u.d = eval(inputs->e, locals);
	}
	return 1;
}

int
c_mouse(Statement *s, Proc *proc, Program *prog)
{
	if(numparam(s->elr) != 4) {
		if(numparam(s->elr)) {
			rule_err(proc, "mouse", "has incorrect number of inputs");
			return 1;
		}
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_mouse(Statement *s, Cell *locals, void **dp)
{
/*
	Prototype:
	    x, y    mouse  [scale1, scale2, scale3, scale4]
	Output Cell 1: Mouse 'x' coordinate
	Output Cell 2: Mouse 'y' coordinate
	Input  Cell 1: Scale value when mouse is furthest left
	Input  Cell 2: Scale value when mouse is furthest right
	Input  Cell 3: Scale value when mouse is lowest
	Input  Cell 4: Scale value when mouse is highest
*/
	Exprlist *outputs = s->el;
	Exprlist *inputs  = s->elr;
	double x, y, scale1, scale2, scale3, scale4;
	double xdiff, ydiff;

	x = eval(outputs->e, locals); outputs = outputs->next;
	y = eval(outputs->e, locals);

	if(numparam(inputs) == 4) {
		scale1 = eval(inputs->e, locals); inputs = inputs->next;
		scale2 = eval(inputs->e, locals); inputs = inputs->next;
		scale3 = eval(inputs->e, locals); inputs = inputs->next;
		scale4 = eval(inputs->e, locals);
	}
	else {
		scale1 = 0.0;
		scale2 = 1.0;
		scale3 = 0.0;
		scale4 = 1.0;
	}
	xdiff = fabs(scale2 - scale1);
	ydiff = fabs(scale4 - scale3);
	if(scale2 < scale1) xdiff = -xdiff;
	if(scale4 < scale3) ydiff = -ydiff;

	get_mouse_coords();
	outputs = s->el;
	LGVAR(outputs->e, locals)->u.d =
	    ((double)mouse_x/SCREENWIDTH  * xdiff) + scale1;
	outputs = outputs->next;
	LGVAR(outputs->e, locals)->u.d =
	    ((1.0 - (double)mouse_y/SCREENHEIGHT) * ydiff) + scale3;

	return 1;
}

int
c_mult(Statement *s, Proc *proc, Program *prog)
{
	if(numparam(s->elr) < 2) {
		rule_err(proc, "mult", "must have at least two inputs");
		return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_mult(Statement *s, Cell *locals, void **dp)
{
	struct	exprlist *inputs;
	struct	expr *output = s->el->e;
	double	mult_adjust;
	double	multval = 1.0, val;
	double	target_left;
	int	all_locked = TRUE, freevar = 0;

	for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
		if(check_lock(inputs->e, locals) == FALSE) {
			all_locked = FALSE;
			break;
		}
	}

	if(all_locked) {
		for(inputs = s->elr; inputs != 0; inputs = inputs->next)
			multval *= eval(inputs->e, locals);
		if(eval(s->el->e, locals) == multval)
			return TRUE;
		else
			return FALSE;
	}

	if(check_lock(output, locals)) {
		target_left = eval(output, locals);
		for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
			val = eval(inputs->e, locals);
			if(val == 0.0)
				LGVAR(inputs->e, locals)->u.d = 1.0;
			else
				multval *= val;
			if(!check_lock(inputs->e, locals)) {
				freevar++;
			}
		}
		mult_adjust = pow(target_left/multval, 1.0/(double)freevar);

		for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
			if(!check_lock(inputs->e, locals))
				LGVAR(inputs->e, locals)->u.d *= mult_adjust;
		}
		return TRUE;
	}
	else {
		for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
			val = eval(inputs->e, locals);
			if(val != 0.0)
				multval *= val;
		}
		if(multval == 1.0)
			multval = 0.0;
		LGVAR(output, locals)->u.d = multval;
		return TRUE;
	}
	return FALSE;
}
