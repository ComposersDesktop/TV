/*
 *	Rule functions
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "tv.h"
#include "rules.h"
#include "midi.h"
#include "xtab.h"

int midi_used = 0;

double
taborval(Expr *e, int idx_in, Cell *locals)
{
	struct	exprlist *idxs;
	struct	table *tab;
	double	*data;
	double	didx;
	int	idx, i;

	switch(e->op) {
	default:
		return eval(e, locals);

	case TIDENT:
		tab = LGVAR(e, locals)->u.table;
		data = tab->mem;
		if(tab == 0) {
		    fprintf(stderr, 
			"Tabula Vigilans: MIDIRule table does not exist!\n");
		    tidy_up(7);
		}
		if(tab->tabdim.dimensions != 1) {
		    fprintf(stderr, "tv: MIDIRule table must have one dimension\n");
		    tidy_up(7);
		}
		idx_in %= tab->tabdim.sizes[0];
		if(idx_in < 0)
			idx_in += tab->tabdim.sizes[0];
		return data[idx_in];

	case '[':
		break;
	}

	tab = LGVAR(e->left, locals)->u.table;
	if(tab == 0) {
	    fprintf(stderr,
		"Tabula Vigilans: MIDIRule table does not exist!\n");
	    tidy_up(7);
	}
	data = tab->mem;
	idxs = e->el;
	for(i = 0; i < tab->tabdim.dimensions; i++) {
	    if(idxs == 0) {
		fprintf(stderr,
		    "Tabula Vigilans: MIDIRule table: not enough indexes\n");
		tidy_up(7);
	    }
	    didx = eval(idxs->e, locals);

	    if(!idxs->wrap) {
		didx -= floor(didx);
		idx = (int)(didx * (double)tab->tabdim.sizes[i]);
	    } else {
		idx = (int)(didx + 0.5);
		if(idx < 0 || idx >= tab->tabdim.sizes[i]) {
			idx %= tab->tabdim.sizes[i];
			if(idx < 0)
				idx += tab->tabdim.sizes[i];
		}
	    }
	    data += idx * tab->tabdim.spaces[i];
	    idxs = idxs->next;
	}
	if(idxs != 0) {
	    fprintf(stderr, "Tabula Vigilans: too many indexes on table\n");
	    tidy_up(7);
	}
	
	data -= idx;
	idx += idx_in;
	idx %= tab->tabdim.sizes[i-1];
	if(idx < 0)
	    idx += tab->tabdim.sizes[i-1];
	return data[idx];
}

int
c_control_out(Statement *s, Proc *proc, Program *prog)
{
	int	i;

	for(i = 0; i < 3; i++) {
	    if(getparam(s->elr, i)->op == TIDENT) {
		rule_err(proc, "control_out",
			"input parameters cannot be tables");
		return 1;
	    }
	}

	if(!midi_used)
		midi_init();

	return checkparam(s->elr, proc, prog);
}

int
e_control_out(Statement *s, Cell *locals, void **dp)
{
	Exprlist *inputs = s->elr;
	int	channel, data1, data2;

	channel = (int)eval(inputs->e, locals);	inputs = inputs->next;
	data1   = (int)eval(inputs->e, locals);	inputs = inputs->next;
	data2   = (int)eval(inputs->e, locals);

	control_out(channel, data1, data2);

	return 1;
}

int
c_midichord(Statement *s, Proc *proc, Program *prog)
{
	int np;

	if((np = numparam(s->elr)) < 4) {
		rule_err(proc, "midichord", "must have at least four input cells");
		return 1;
	}
	if(np > 6) {
		rule_err(proc, "midichord",
				"cannot have more than six input cells");
		return 1;
	}
	if((getparam(s->elr, 3)->op) == TIDENT) {
		rule_err(proc, "midichord",
				"duration parameter cannot be a table");
		return 1;
	}

	if(!midi_used)
		midi_init();

	return checkparam(s->elr, proc, prog);
}

int
i_midichord(void **dp)
{
	*(long *)dp = 0;
	return 0;
}

int
e_midichord(Statement *s, Cell *locals, void **dp)
{
	long	*cntptr = (long *)dp;
	struct	exprlist *inputs = s->elr;
	struct	expr *channel, *pitch, *velocity;
	double	duration;
	int	arp, toplay, idx, numnotes;
	TICK	delay;
	TICK	now = current_time();

	channel =  inputs->e;		inputs = inputs->next;
	pitch =    inputs->e;		inputs = inputs->next;
	velocity = inputs->e;		inputs = inputs->next;
	duration = eval(inputs->e, locals); inputs = inputs->next;
	numnotes = (inputs != 0) ? (int)eval(inputs->e, locals) : 1;
	if(inputs != 0) inputs = inputs->next;
	arp = (int)((inputs != 0 ? eval(inputs->e, locals) : 0));
	if(arp && numnotes > 1)
	    delay = (TICK)((TICKSPERSEC * duration) / numnotes);
	else
	    delay = 0;
	toplay = numnotes - *cntptr;

	if(toplay < 1)
		return -*cntptr;

	for(idx = 0; idx < toplay; idx++) {
		++*cntptr;
		midi_note(now + (idx*delay),
			now+(TICK)(TICKSPERSEC * duration),
			(int)(taborval(channel, idx, locals)),
			(int)(taborval(pitch, idx, locals)),
			(int)(taborval(velocity, idx, locals)),
			cntptr);
	}
	return *cntptr;
}

int
c_midiecho(Statement *s, Proc *proc, Program *prog)
{
	if(!midi_used)
		midi_init();

	return checkparam(s->elr, proc, prog);
}

struct echodata {
	int channel;
	int pitch;
	int velocity;
};

int
i_midiecho(void **dp)
{
    //	struct echodata *ed;

	*dp = emalloc(sizeof(struct echodata));
    //	ed = (struct echodata *)*dp;

	return 1;
}

int
e_midiecho(Statement *s, Cell *locals, void **dp)
{
	struct	exprlist *inputs = s->elr;
	struct	echodata *ed = *(struct echodata **)dp;
	int	channel, pitch, velocity;
	TICK	now = current_time();

	channel =  (int)eval(inputs->e, locals);	inputs = inputs->next;
	pitch =    (int)eval(inputs->e, locals);	inputs = inputs->next;
	velocity = (int)eval(inputs->e, locals);

	if((pitch != (unsigned char)ed->pitch) ||
	   (velocity != (unsigned char)ed->velocity) ||
	   (channel != (unsigned char)ed->channel)) {
	    if(velocity) {
		midi_event(note_on, now, channel, pitch, velocity, 0);
		ed->channel = channel;
		ed->pitch = pitch;
		ed->velocity = velocity;
	    }
	    else
		midi_event(note_off, now, channel, pitch, 0, 0);
	}
	return 1;
}

int
c_midiin(Statement *s, Proc *proc, Program *prog)
{
	if(!midi_used)
		midi_init();

	return checkparam(s->el, proc, prog);
}

int
i_midiin(void **dp)
{
	*(long *)dp = 0;
	return 0;
}

int
e_midiin(Statement *s, Cell *locals, void **dp)
{
	return midiin(s, locals, dp);	
}

int
c_midiout(Statement *s, Proc *proc, Program *prog)
{
	int np = numparam(s->elr);

	if(np < 4) {
		rule_err(proc, "midiout", "must have at least four input cells");
		return 1;
	}
	if(np > 5) {
		rule_err(proc, "midiout",
				"cannot have more than five input cells");
		return 1;
	}
	if((np == 5) && (getparam(s->elr, 4)->op == TIDENT)) {
		rule_err(proc, "midiout", "fifth parameter cannot be a table");
		return 1;
	}

	if(!midi_used)
		midi_init();

	return checkparam(s->elr, proc, prog);
}

int
i_midiout(void **dp)
{
	*(long *)dp = 0;
	return 0;
}


int
e_midiout(Statement *s, Cell *locals, void **dp)
{
	long	*cntptr = (long *)dp;
	Exprlist *inputs = s->elr;
	Expr *channel, *pitch, *velocity, *duration;
	int	density, toplay, idx;
	TICK	now = current_time();

	channel =  inputs->e;		inputs = inputs->next;
	pitch =    inputs->e;		inputs = inputs->next;
	velocity = inputs->e;		inputs = inputs->next;
	duration = inputs->e;		inputs = inputs->next;
	density = (inputs != 0) ? (int)eval(inputs->e, locals) : 1;

	if(isatable(channel)
	 ||isatable(pitch)
	 ||isatable(velocity)
	 ||isatable(duration))
		toplay = density - *cntptr;
	else
		toplay = (density > *cntptr) ? 1 : 0;

	if(toplay < 1)
		return -*cntptr;

	for(idx = 0; idx < toplay; idx++) {
		++*cntptr;
		midi_note(now,
			now+(TICK)(TICKSPERSEC * taborval(duration, idx, locals)),
			(int)(taborval(channel, idx, locals)),
			(int)(taborval(pitch, idx, locals)),
			(int)(taborval(velocity, idx, locals)),
			cntptr);
	}
	return *cntptr;
}

int
c_midiset(Statement *s, Proc *proc, Program *prog)
{
	return checkparam(s->elr, proc, prog);
}

int
e_midiset(Statement *s, Cell *locals, void **dp)
{
	Exprlist *inputs = s->elr;
	int channel, instr;

	channel = (int)eval(inputs->e, locals);		inputs = inputs->next;
	instr =  (int)eval(inputs->e, locals);

	midiset(channel, instr);
	return 1;
}

int
c_pitchbend(Statement *s, Proc *proc, Program *prog)
{
	return checkparam(s->elr, proc, prog);
}

int
e_pitchbend(Statement *s, Cell *locals, void **dp)
{
	Exprlist *inputs = s->elr;
	int channel, bend;

	channel = (int)eval(inputs->e, locals);		inputs = inputs->next;
	bend =  (int)eval(inputs->e, locals);

	pitchbend(channel, bend);
	return 1;
}

int
c_schedule(Statement *s, Proc *proc, Program *prog)
{
	if(getparam(s->elr, 4)->op == TIDENT) {
		rule_err(proc, "schedule", "fifth parameter cannot be a table");
		return 1;
	}

	if(!midi_used)
		midi_init();

	return checkparam(s->elr, proc, prog);
}

int
i_schedule(void **dp)
{
	*(long *)dp = 0;
	return 0;
}

int
e_schedule(Statement *s, Cell *locals, void **dp)
{
    /* Prototype is:
	schedule channel, pitch, velocity, duration, timepoint
    */
	long	*cntptr = (long *)dp;
	Exprlist *inputs = s->elr;
	Expr *channel, *pitch, *velocity, *duration;
	int	idx = 0;
	TICK	now = current_time(), timepoint;

	channel =  inputs->e;		inputs = inputs->next;
	pitch =    inputs->e;		inputs = inputs->next;
	velocity = inputs->e;		inputs = inputs->next;
	duration = inputs->e;		inputs = inputs->next;
	timepoint = (TICK)(eval(inputs->e, locals) * TICKSPERSEC);
/**/	if(timepoint >= 0) {
	    timepoint += now;
	    ++*cntptr;
	    midi_note(timepoint,
		timepoint+(TICK)(TICKSPERSEC * taborval(duration, idx, locals)),
		(int)(taborval(channel, idx, locals)),
		(int)(taborval(pitch, idx, locals)),
		(int)(taborval(velocity, idx, locals)),
		cntptr);
	}
	return *cntptr;
}
