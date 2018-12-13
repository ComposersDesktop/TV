/*
 *	TV Midi support routines
 */
#include <stdlib.h>
#include <stdio.h>

#include "tv.h"
#include "rules.h"
#include "midi.h"

static struct midi_event *midiq;

void
midi_init(void)
{
	midiq = 0;
	midiopen();
	midi_used = 1;
}

void
midi_event(enum event_type type,
	TICK time, int channel,
	int note, int velocity,
	long *inuseptr)
{
	struct midi_event **mp = &midiq;
	struct midi_event *me = (struct midi_event *)emalloc(sizeof(struct midi_event));

	me->type = type;
	me->time = time;
	me->channel = channel;
	me->note = note;
	me->velocity = velocity;
	me->inuseptr = inuseptr;

	while(*mp != 0 && (*mp)->time < time)
		mp = &(*mp)->next;

	me->next = *mp;
	*mp = me;
}

void
midi_poll(void)
{
	TICK now;
	struct midi_event **mp;

	if(!midi_used) return;

	now = current_time();
	mp = &midiq;

	while(*mp != 0 && (*mp)->time <= now) {
		struct midi_event *m = *mp;
		if(m->note > 0)
			midiout(m);
		if(m->inuseptr != 0)
			--*m->inuseptr;
		*mp = m->next;
		free((void *)m);
	}
}

void
midi_cleanup(int wait)
{
	if(wait) {
		while(midiq != 0)
			midi_poll();
	}

	while(midiq != 0) {
		struct midi_event *m = midiq;

		if(midiq->type == note_off)
			midiout(midiq);
		free((void *)midiq);
		midiq = m;
	}
	midiclose();
}

/*
 *	nicer interface routines
 */
void
midi_note(TICK start, TICK finish,
		int channel, int note,
		int velocity, long *inuseptr)
{
	midi_event(note_on, start, channel, note, velocity, 0);
	midi_event(note_off, finish, channel, note, 0, inuseptr);
}
