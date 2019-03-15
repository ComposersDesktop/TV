/*
 *	TABULA VIGILANS: Rule functions, Set 2
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <process.h>
#endif
#include "tv.h"
#include "rules.h"
#include "xtab.h"
#include "midi.h"

int
c_pop(Statement *s, Proc *proc, Program *prog)
{
	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_pop(Statement *s, Cell *locals, void **dp)
{
/*
	Implements a(next) = r * a * (1.0 - a)
*/
	Expr *output = s->el->e;
	Exprlist *inputs = s->elr;
	double val, r;

	val = eval(output, locals);
	if(val == 0)
	    val = rand();		/* Give a starting value */
	if(fabs(val) > 1.0)		/* Check starting value */
	    val = 1.0/fabs(val);

	r = eval(inputs->e, locals);
	if(r > 3.999) r = 3.999;	/* Check limiting value */

	LGVAR(output, locals)->u.d = r * val * (1.0 - val);

	return 1;
}

int
c_print(Statement *s, Proc *proc, Program *prog)
{
	return checkexpr(s->elr->e, proc, prog);
}

int
e_print(Statement *s, Cell *locals, void **dp)
{
	double d;
	int field1 = 6;
	int field2 = 2;
	Exprlist *el = s->elr;

	d = eval(el->e, locals);
	if(el->next != 0) {
		el = el->next;
		field1 = (int) eval(el->e, locals);
	}
	if(el->next != 0) {
		el = el->next;
		field2 = (int) eval(el->e, locals);
	}
	printf("%*.*lf", field1, field2, d);
	fflush(stdout);

	return 1;
}


int
c_probe(Statement *s, Proc *proc, Program *prog)
{
	if(numparam(s->elr) < 1) {
		rule_err(proc, "probe", "must have at least one input");
		return 1;
	}

	return checkparam(s->elr, proc, prog);
}

int
e_probe(Statement *s, Cell *locals, void **dp)
{
	Exprlist *el;

	for(el = s->elr; el != 0; el = el->next) {
		printf(" %7.2lf", eval(el->e, locals));
/*		if(el->next != 0)
			printf(" ");
*/	}
	printf("\r");
	fflush(stdout);
	return 1;
}

int
c_probi(Statement *s, Proc *proc, Program *prog)
{
	if(numparam(s->elr) < 1) {
		rule_err(proc, "probi", "must have at least one input");
		return 1;
	}

	return checkparam(s->elr, proc, prog);
}

//R.O 6 99
static int val_round(double val)
{
	int k;
	k = (int)(fabs(val)+0.5);
	if(val < 0.0)
		k = -k;
	return k;
}
#ifdef JPFFSRC
int
e_probi(Statement *s, Cell *locals, void **dp)
{
	Exprlist *el;
	long   val;

	for(el = s->elr; el != 0; el = el->next) {
		val = (long)(eval(el->e, locals));
		if(val >= 0) val += 0.5;
		printf("%5ld", val);
		if(el->next != 0)
			printf("\t");
	}
	printf("\r");
	fflush(stdout);
	return 1;
}
#else
/* RWD: this is what we had - looks correct, no compiler warnings! */
int
e_probi(struct statement *s, struct cell *locals, void **dp)
{
	struct exprlist *el;
	long val;

	for(el = s->elr; el != 0; el = el->next) {
		val = val_round(eval(el->e, locals));		
		printf("%5ld",val);
		if(el->next != 0)
			printf("\t");
	}
	printf("\r");
	fflush(stdout);
	return 1;
}


#endif
struct segdata {
	int target_reached;
	TICK starttime;
	TICK targettime;
	TICK dur;
};

int
c_seg(Statement *s, Proc *proc, Program *prog)
{
	int i;

	if(numparam(s->el) != 1) {
		rule_err(proc, s->rule->picture, "must have one output cell");
		return 1;
	}
	if((i = numparam(s->elr)) != 3) {
		rule_err(proc, s->rule->picture,
			"must have three input cells");
		return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
i_seg(void **dp)
{
	struct segdata *sd;

	*dp = emalloc(sizeof(struct segdata));
	sd = (struct segdata *)*dp;

	sd->target_reached = 1;
	return 1;
}

int
e_seg(Statement *s, Cell *locals, void **dp)
{
/*
	Output Cell:  Current Output Value;
	Input Cell 1 (Required): User-supplied segment duration
	Input Cell 2 (Optional): scale value 1
	Input Cell 3 (Optional): scale value 2
*/
	double comp, dur, scale1, scale2, diff;
	Exprlist *inputs = s->elr;
	Expr *out  = s->el->e;
	unsigned long timevalue = current_time();
	struct segdata *sd = *(struct segdata **)dp;

	if(sd->target_reached) {
	    sd->starttime = timevalue;
	    dur = eval(inputs->e, locals);
	    inputs = inputs->next;
	    sd->targettime = (unsigned long) (timevalue + (dur * TICKSPERSEC));
	    sd->dur = (unsigned long) dur * TICKSPERSEC;		/* Dur as long */
	    sd->target_reached = FALSE;
	}

	inputs = s->elr->next;
	scale1 = eval(inputs->e, locals);
	inputs = inputs->next;
	scale2 = eval(inputs->e, locals);
	diff = scale2 - scale1;

	comp = (double)(timevalue - sd->starttime) / sd->dur;
	comp = (comp < 0.0 ? scale1 : comp);
	LGVAR(out, locals)->u.d =
		(comp > 1.0 ? diff+scale1 : comp * diff + scale1);
	if(timevalue >= sd->targettime) {
	    sd->target_reached = TRUE; /* ready for next segment */
	}
	return (sd->target_reached);
}

int
c_store(Statement *s, Proc *proc, Program *prog)
{
    if(numparam(s->elr) < 1) {
	rule_err(proc, proc->name->picture, "must have at least one input");
	return 1;
    }

    return checkparam(s->elr, proc, prog);
}

int
e_store(Statement *s, Cell *locals, void **dp)
{
    char string[MAXSTRING];
    char temp[MAXSTRING];
    int	 check, mode;
    Exprlist *el;
    Sfile *f = sfile;

    if(strcmp(s->rule->picture, "store") == 0)
	mode = 0;
    else if(strcmp(s->rule->picture, "stori") == 0)
	mode = 1;
    else if(strcmp(s->rule->picture, "stor4") == 0)
	mode = 2;
    else if(strcmp(s->rule->picture, "stor6") == 0)
	mode = 3;
    else if(strcmp(s->rule->picture, "storf") == 0)
	mode = 4;

    if (mode != 4) {
      while (f!=NULL) {
        if (f->fnum==0) break;
        f = f->next;
      }
      if (f==NULL) {
        fprintf(stderr, "Storefile not allocated for storstr");
        tidy_up(5);
      }
    }

    strcpy(string, "");
    strcpy(temp, "");

    if(mode == 0) {		/* STORE */
	for(el = s->elr; el != 0; el = el->next) {
	    sprintf(temp, "%.*lf", decdig, (eval(el->e, locals)));
	    if(el->next != 0) {
              strncat(temp, "\t", MAXSTRING-strlen(temp));
              temp[MAXSTRING-1] = '\0';
            }
	    strcat(string, temp);
	}
    }
    else if(mode == 1) {	/* STORI */
	for(el = s->elr; el != 0; el = el->next) {
	    if((long)(eval(el->e, locals) < 0))
/*SIGNCHECK*/	sprintf(temp, "%ld", (long)(eval(el->e, locals)-0.5));
	    else
		sprintf(temp, "%ld", (long)(eval(el->e, locals)+0.5));
/**/	    if(el->next != 0)
		strcat(temp, "\t");
	    strcat(string, temp);
	}
    }
    else if(mode == 2) {		/* STOR4 */
	for(el = s->elr; el != 0; el = el->next) {
	    sprintf(temp, "%.4lf", (eval(el->e, locals)));
	    if(el->next != 0)
		strcat(temp, "\t");
	    strcat(string, temp);
	}
    }
    else if(mode == 3) {		/* STOR6 */
	for(el = s->elr; el != 0; el = el->next) {
	    sprintf(temp, "%.6lf", (eval(el->e, locals)));
	    if(el->next != 0)
		strcat(temp, "\t");
	    strcat(string, temp);
	}
    }
    else if(mode == 4) {		/* STORF */
        int n = (int) (eval(s->elr->e, locals));
        f = sfile;
        while (f) {
          if (f->fnum==n) break;
		  f = f->next;	/* RO added from JF's memo 10/12/04 */
        }
        if (f==NULL) {
          fprintf(stderr, "Storefile not allocated for storstr");
          tidy_up(5);
        }
	for(el = s->elr->next; el != 0; el = el->next) {
	    sprintf(temp, "%.*lf", decdig, (eval(el->e, locals)));
	    if(el->next != 0)
		strcat(temp, "\t");
	    strcat(string, temp);
	}
    }

    strcat(f->store_buf, string);
    if(strlen(f->store_buf) > 1024) {
	check = write(f->storefd, f->store_buf, 1024);
	if(check != 1024)
	{
	    fprintf(stderr, "\nSTORE: Storefile write failure!");
	    tidy_up(5);
	}
	memcpy(f->store_buf, f->store_buf + 1024, 1024);
    }
    return 1;
}

int
c_sum(Statement *s, Proc *proc, Program *prog)
{
	if(numparam(s->elr) < 2) {
	    rule_err(proc, "sum", "must have at least two inputs");
	    return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
e_sum(Statement *s, Cell *locals, void **dp)
{
	struct	exprlist *inputs;
	struct	expr *output = s->el->e;
	double	diff_adjust;
	double	sumval = 0.0;
	double	target_left, target_right;
	int	all_locked = TRUE;

	for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
	    if(check_lock(inputs->e, locals) == FALSE) {
		all_locked = FALSE;
		break;
	    }
	}

	if(all_locked) {
	    for(inputs = s->elr; inputs != 0; inputs = inputs->next)
		sumval += eval(inputs->e, locals);
	    if(eval(s->el->e, locals) == sumval)
		return TRUE;
	    else
		return FALSE;
	}	    

	if(check_lock(output, locals)) {
	    target_left = eval(output, locals);
	    target_right = 0;
	    for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
		if(check_lock(inputs->e, locals))
		    target_left -= eval(inputs->e, locals);
		else {
		    target_right += eval(inputs->e, locals);
		}
	    }
	    diff_adjust = target_left/target_right;

	    for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
		if(check_lock(inputs->e, locals) == 0) 
		    LGVAR(inputs->e, locals)->u.d *= diff_adjust;
	    }
	    return TRUE;
	}
	else {
	    for(inputs = s->elr; inputs != 0; inputs = inputs->next)
		sumval += eval(inputs->e, locals);
	    LGVAR(output, locals)->u.d = sumval;
	    return TRUE;
	}
	return FALSE;
}

int
c_swap(Statement *s, Proc *proc, Program *prog)
{
	return checkparam(s->elr, proc, prog);
}

int
e_swap(Statement *s, Cell *locals, void **dp)
{
/*
	Swap values of input cells
*/
	Exprlist *inputs = s->elr;
	int	i = 1;
	double	val[2];

	if(check_lock(inputs->e, locals) ||
	    check_lock(inputs->next->e, locals))
		return 0;

	val[0] = eval(inputs->e, locals);
	inputs = inputs->next;
	val[1] = eval(inputs->e, locals);

	for(inputs = s->elr; inputs != 0; inputs = inputs->next) {
	    if(inputs->e->op == '[')
		*dereftab(LGVAR(inputs->e->left,locals)->u.table,
			inputs->e->el, locals)	= val[i--];
	    else
		LGVAR(inputs->e, locals)->u.d = val[i--];
	}
	return 1;
}

int
c_time(Statement *s, Proc *proc, Program *prog)
{
	int rhs = numparam(s->elr);

	if(rhs > 1) {
		rule_err(proc, "time", "can only have one input cell");
		return 1;
	}
	if(rhs > 0 && s->elr->e->op != IDENT) {
		rule_err(proc, "time",
			"right hand parameter must be identifier");
		return 1;
	}

	return checkparam(s->elr, proc, prog) + checkexpr(s->el->e, proc, prog);
}

int
i_time(void **dp)
{
	*(TICK *)dp = 0;
	return 0;
}

int
e_time(Statement *s, Cell *locals, void **dp)
{
	unsigned long timevalue = current_time();
	unsigned long start;
	Expr *dest  = s->el->e;
	TICK *starttime = (TICK *)dp;

	if(s->elr != 0) {
		double *sd = &LGVAR(s->elr->e, locals)->u.d;
		if(*sd == 0.0)
			*sd = start = timevalue;
		else
			start = (unsigned long)*sd;
	} else {
		if(*starttime == 0)
			*starttime = timevalue;
		start = *starttime;
	}

	LGVAR(dest,locals)->u.d = (timevalue - start)/(double)TICKSPERSEC;

	return 1;
}

int
c_trigger(Statement *s, Proc *proc, Program *prog)
{
	Expr *rhs = s->elr->e;

	if(s->el->e->op != IDENT) {
		rule_err(proc, "trigger", "output must be identifier");
		return 1;
	}
	if((rhs->op != IDENT) && rhs->op != NUMBER) {
		rule_err(proc, "trigger", "input parameter must be identifier");
		return 1;
	}

	return checkparam(s->el, proc, prog) + checkparam(s->elr, proc, prog);
}

int
i_trigger(void **dp)
{
	*(TICK *)dp = 0;
	return 0;
}

int
e_trigger(Statement *s, Cell *locals, void **dp)
{
/*	Prototype:	tr1 trigger timevalue 
	- tr1 is made positive each time 'timevalue' has elapsed
*/
	Expr *dest  = s->el->e;
	TICK timevalue = current_time();
	TICK start;
	TICK *triggerpoint = (TICK *)dp;
	long trigger = *triggerpoint - timevalue;

	if(check_lock(dest, locals))
		return 0;

	if(*triggerpoint == 0 || trigger <= 0) {
	    LGVAR(dest,locals)->u.d = 1.0;
	    start = timevalue;
	    *(TICK *)dp = (unsigned long)((eval(s->elr->e, locals) * (double)TICKSPERSEC)
					+ start);
	} else {
	    LGVAR(dest,locals)->u.d = 0.0;
	}
	    
	return 1;
}

int
e_unlock(Statement *s, Cell *locals, void **dp)
{
	Exprlist *e = s->elr;

	while(e != 0) {
		LGVAR(e->e,locals)->lock = 0;
		e = e->next;
	}
	return 0;
}

int
c_wait(Statement *s, Proc *proc, Program *prog)
{
	if(s->elr->e->op != IDENT && s->elr->e->op != NUMBER) {
		rule_err(proc, "wait", "input parameter must be a duration");
		return 1;
	}

	return checkexpr(s->elr->e, proc, prog);
}


int
i_wait(void **dp)
{
	*(TICK *)dp = 0;
	return 0;
}

int
e_wait(Statement *s, Cell *locals, void **dp)
{
	Expr *w  = s->elr->e;
	TICK waittime;

	waittime = current_time() + (TICK)(eval(w, locals) * TICKSPERSEC);

	while(current_time() < waittime)
		midi_poll();
	return 1;
}
