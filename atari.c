#include <stdio.h>
#include <osbind.h>
#include "tv.h"
#include "rules.h"
#include "midi.h"

#define NEXT_MIDI (unsigned char)Bconin(3)&0xFF

unsigned long _STACK = 20*1024;

short	work_in[12],
	work_out[57];
short	mouse_button, screen_handle;
short	mouse_x, mouse_y;

void
cls(void)
{
	printf("\033E\nTabula Vigilans:\n");
}

TICK
current_time()
{
	register void *savessp = Super(NULL);
	register unsigned long timevalue = (unsigned long)*((TICK*)0x4ba); 

	Super(savessp);
	return timevalue;
}

void
gem_init(void)
{
	short	i;

	appl_init();
	for (i=1; i<10; work_in[i++]=1);
	work_in[0] = Getrez() + 2; /* opens driver for current resolution */
	work_in[10]=2;		   /* choose raster coordinates */
	v_opnvwk(work_in, &screen_handle, work_out);
}

void
get_mouse_coords()
{
	vq_mouse(screen_handle, &mouse_button, &mouse_x, &mouse_y);
}

unsigned char
get_next_midi(int type, int *valid)
	/* type:  Return immediately, or stay? */
	/* valid: Indicates whether returned with a valid byte */
{
	unsigned char inmidi;
	do {
	    if(type == IMMEDIATE) {
		if(Bconstat(3)) {	/* Only get a byte if it's there */
		    inmidi = NEXT_MIDI;	/* collect next MIDI byte */
		}
		else {	/* No more MIDI in buffer */
		    inmidi = 0;	/* To break out of loop */
		    *valid = 0; /* Inform calling function not to use 'inmidi' */
		}
	    }
	    else {
		inmidi = NEXT_MIDI;	/* STAY for next byte */
	    }
	} while((inmidi & 0xF8) == 0xF8); /* REAL-TIME */

	return(inmidi);
}

int
midiin(struct statement *s, struct cell *locals, void **dp)
{
    struct exprlist *data = s->el;
    static unsigned char last_status = 0;
    unsigned char midibyte;
    int	note_received = NOTHING;
    int	more_to_get = 1;
    int	valid = 1;

    while(more_to_get) {
	if(Bconstat(3)) {	/* If there's MIDI present */
	    midibyte = get_next_midi(IMMEDIATE,&valid); /* Fetch the byte */

	    if(valid) {		/* If there's still a valid byte there */
		if(midibyte >= 128) {	/* It's a STATUS byte */
		    last_status = midibyte;
		    if((midibyte&0xE0) == 0x80) {   /* Note Event */
			note_received = NOTE_EVENT;
			LGVAR(data->e,locals)->u.d = (double)(midibyte&0x0F);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)get_next_midi(STAY,&valid);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)get_next_midi(STAY,&valid);
		    }
		    else if((midibyte&0xF0) == 0xD0) {	/* Aftertouch */
			note_received = AFTERTOUCH;
			LGVAR(data->e,locals)->u.d = (double)(midibyte&0x0F);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)get_next_midi(STAY,&valid);
		    }
		    else if((midibyte&0xF0) == 0xB0) {	/* Controller Info */
			note_received = CONTROLLER;
			LGVAR(data->e,locals)->u.d = (double)(midibyte&0x0F);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)get_next_midi(STAY,&valid);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)get_next_midi(STAY,&valid);
		    }
		}
		else {	/* This is a DATA BYTE */
		    if((last_status&0xE0) == 0x80) { /* Last Status was Note Event */
			note_received = NOTE_EVENT;
			LGVAR(data->e,locals)->u.d = (double)(last_status&0x0F);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)midibyte;
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)get_next_midi(STAY,&valid);
		    }
		    else if((last_status&0xF0) == 0xD0) {/* Last Status was Aftertouch */
			note_received = AFTERTOUCH;
			LGVAR(data->e,locals)->u.d = (double)(last_status&0x0F);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)midibyte;
		    }
		    else if((last_status&0xF0) == 0xB0) { /* Last Status was Controller */
			note_received = CONTROLLER;
			LGVAR(data->e,locals)->u.d = (double)(last_status&0x0F);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)midibyte;
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)get_next_midi(STAY,&valid);
		    }
		}
	    }      

	    if(note_received != NOTHING) {	/* If event received */
		more_to_get = 0;	/* Stop now - we've got a note event */
		if((last_status&0xF0) == 0x80) { /* If 'twas a Note */
			data = data->next;
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double)0.;
			/* Turn any note OFF's into velocity 0 */
		}
	    }
	}
	else more_to_get = 0;  /* Stop now, as there's no more MIDI */ 
    }
    return note_received;
}

void
perf_time(int report) /* stores time of performance */
{
	double secs;
	unsigned long mins = 0L, hours = 0L, days = 0L;

	register unsigned long timevalue = current_time();
	static short	first = TRUE;
	static unsigned long	start_systime;

	if(first) {
	    start_systime = timevalue;
	    first = FALSE;
	}

	if(report) {
	    secs = (double)(timevalue - start_systime)/(double)TICKSPERSEC;
	    mins = 0.5 + (long)(secs/60.0);
	    secs -= mins * 60;
	    if(mins > 60) {
		hours = mins/60;
		mins %= 60;
	    }
	    if(hours > 24) {
		days = hours/24;
		hours %= 24;
	    }

	    if(days) {
		if(days == 1) {
		    if(hours == 1)
			printf("\nPerformance Time: %3ld day, %2ld hr. %2ld' %.2lf\"\n",
			    days, hours, mins, secs);
			else 	printf("\nPerformance Time: %3ld day, %2ld hrs. %2ld' %.2lf\"\n",
			    days, hours, mins, secs);
		} else {
		    if(hours == 1)
			printf("\nPerformance Time: %3ld days, %2ld hr. %2ld' %.2lf\"\n",
			    days, hours, mins, secs);
		    else printf("\nPerformance Time: %3ld days, %2ld hrs. %2ld' %.2lf\"\n",
			    days, hours, mins, secs);
		    }
		}
		else if(hours) {
		    if(hours == 1)
			printf("\nPerformance Time: %2ld hr. %2ld' %.2lf\"\n",
			    hours, mins, secs);
		    else printf("\nPerformance Time: %2ld hrs. %2ld' %.2lf\"\n",
			    hours, mins, secs);
		}
		else if(mins)
		    printf("\nPerformance Time: %2ld' %.2lf\"\n",
			mins, secs);
		else
		    printf("\nPerformance Time: %.2lf\"\n", secs);
	}
}

void
inits(void)
{
	Cursconf(0,0);
	gem_init();
	while(Bconstat(2))Bconin(2);
	    /* Clear keyboard buffer of any previously typed characters */
	srand48(Gettime());
}

void
mach_cleanup()
{
	Cursconf(1, 1);
}

/*
 *	midi support routines
 */

void
control_out(int channel, int data1, int data2)
{
	unsigned char buf[3];

	buf[0] = 0xB0 + (unsigned char)channel;
	buf[1] = (unsigned char)data1;
	buf[2] = (unsigned char)data2;

	Midiws(2, buf);
}

void
midiopen()
{
	while(Bconstat(3))Bconin(3);
		/* Clear MIDI buffer of any spurious notes */
	/* nothing else to do on the Atari */
}

void
midiclose()
{
	unsigned char	buf[3];
	int i;

	for(i = 0; i < 16; i++)
	{
	    buf[0] = 0xB0 + i;	/* ALL NOTES OFF */
	    buf[1] = 0x7B;
	    buf[2] = 0;
	    Midiws(2, buf);
	}
}

void
midiout(struct midi_event *m)
{
	unsigned char buf[3];

	buf[0] = 0x90 + m->channel;
	buf[1] = m->note;
	buf[2] = m->velocity;

	Midiws(2, buf);
}

void
midiset(int channel, int instr)
{
	unsigned char buf[2];

	buf[0] =  0xc0 + (unsigned char)channel;
	buf[1] =  (unsigned char)instr;
	Midiws(1, buf);
}

void
pitchbend(int channel, int bend)
{
	unsigned char buf[3];

	buf[0] =  0xE0 + (unsigned char)channel;
	buf[1] =  (unsigned char)0;
	buf[2] =  (unsigned char)bend;
	Midiws(2, buf);
}
