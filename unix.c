/*
 *	unix support
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include "tv.h"
#include "rules.h"
#include "midi.h"
#include <portmidi.h>
#include <porttime.h>
/* #ifndef __MACH__ */
/* #include <stropts.h> */
/* #endif */
#include <poll.h>

static int indev = 0;
static int outdev = 0;

TICK
current_time(void)
{
        struct timeval tv;
        struct timezone tz;

	if(gettimeofday(&tv, &tz) < 0) {
		fprintf(stderr, "tv: gettimeofday failed!\n");
		exit(1);
	}
	tv.tv_sec &= 0x0000FFFF;
	return tv.tv_sec*200 + tv.tv_usec/(1000000/200);
}

void
perf_time(int report) /* stores time of performance */
{
	static int first = 0;
	static time_t starttime;

	double secs;
	long mins, hours = 0L, days = 0L;

	if(!first) {
		first = 1;
		starttime = current_time();
	}

	if(report) {
		secs = (current_time() - starttime)/(double)TICKSPERSEC;
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
				else	printf("\nPerformance Time: %3ld days, %2ld hrs. %2ld' %.2lf\"\n",
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

int finish;

static void
hdlr(int sig)
{
	finish = 1;
	signal(SIGINT, hdlr);
}

void
mach_cleanup(void)
{
}



/*
 *	midi support routines
 */



#define DRIVER_INFO NULL
#define TIME_PROC ((int (*)(void *)) Pt_Time)
#define TIME_INFO NULL
#define TIME_START Pt_Start(1, 0, 0) /* timer started w/millisecond accuracy */
#define NOTEOFF_TYPE  0x80
#define NOTEON_TYPE   0x90
#define POLYAFT_TYPE  0xA0
#define CONTROL_TYPE  0xB0
#define PROGRAM_TYPE  0xC0
#define AFTOUCH_TYPE  0xD0
#define PCHBEND_TYPE  0xE0
#define SYSTEM_TYPE   0xF0

static int portMidi_getDeviceCount(int output)
{
    int           i, cnt1, cnt2;
    PmDeviceInfo  *info;

    cnt1 = (int)Pm_CountDevices();
    if (cnt1 < 1)
      return cnt1;      /* no devices */
    cnt2 = 0;
    for (i = 0; i < cnt1; i++) {
      info = (PmDeviceInfo*)Pm_GetDeviceInfo((PmDeviceID) i);
      if (output && info->output)
        cnt2++;
      else if (!output && info->input)
        cnt2++;
    }
    return cnt2;
}

static int portMidi_getRealDeviceID(int dev, int output)
{
    int           i, j, cnt;
    PmDeviceInfo  *info;

    cnt = (int)Pm_CountDevices();
    i = j = -1;
    while (++i < cnt) {
      info = (PmDeviceInfo*)Pm_GetDeviceInfo((PmDeviceID) i);
      if ((output && !(info->output)) || (!output && !(info->input)))
        continue;
      if (++j == dev)
        return i;
    }
    return -1;
}

static PmDeviceInfo *portMidi_getDeviceInfo(int dev, int output)
{
    int i;

    i = portMidi_getRealDeviceID(dev, output);
    if (i < 0)
      return NULL;
    return ((PmDeviceInfo*)Pm_GetDeviceInfo((PmDeviceID) i));
}

void list_mididevs(void)
{
    int           i, cnt;
    PmDeviceInfo  *info;

    cnt = portMidi_getDeviceCount(0);
    if (cnt >= 1) {
      printf("The available MIDI in devices are:\n");
      for (i = 0; i < cnt; i++) {
        info = portMidi_getDeviceInfo(i, 0);
        if (info->interf != NULL)
          printf(" %3d: %s (%s)\n", i, info->name, info->interf);
        else
          printf(" %3d: %s\n", i, info->name);
      }
    }
    else
      printf("No available MIDI in devices\n");
    cnt = portMidi_getDeviceCount(1);
    if (cnt >= 1) {
      printf("The available MIDI out devices are:\n");
      for (i = 0; i < cnt; i++) {
        info = portMidi_getDeviceInfo(i, 1);
        if (info->interf != NULL)
          printf(" %3d: %s (%s)\n", i, info->name, info->interf);
        else
          printf(" %3d: %s\n", i, info->name);
      }
    }
    else
      printf("No available MIDI out devices\n");
}

void
inits(int dev_midiin,int dev_midiout)
{
    int         cnt_indev,cnt_outdev; /* RWD count ins and outs separately*/
    srand48(time(0));

    finish = 0;
    if(signal(SIGINT, SIG_IGN) != SIG_IGN)
      signal(SIGINT, hdlr);
    cnt_indev = portMidi_getDeviceCount(0);
    if (cnt_indev==0) {
      fprintf(stderr,"\nNo MIDI IN device found\n");
    }
    else {
      PmEvent     buffer;
      PmError     retval;
      PmDeviceInfo *info;
      PortMidiStream *midistream;
      
      /* look up device in list */
#ifndef __MACH__
		if (dev_midiin < 0 || dev_midiin >= cnt_indev) {
        printf("error: device number is out of range\n");
        exit(1);
      }
#endif
      info = portMidi_getDeviceInfo(dev_midiin, 0);
      if (info->interf != NULL)
      printf("PortMIDI: selected input device %d: '%s' (%s)\n",
             dev_midiin, info->name, info->interf);
      else
        printf("PortMIDI: selected input device %d: '%s'\n",
               dev_midiin, info->name);
		indev = dev_midiin;
		
#ifdef NOTDEF
      retval = Pm_OpenInput(&midistream,
                            (PmDeviceID) portMidi_getRealDeviceID(dev_midiin, 0),
                            NULL, 512L, (PmTimeProcPtr) NULL, NULL);
      if (retval != pmNoError) {
        printf("error opening input device %d: %s",
              dev_midiin, Pm_GetErrorText(retval));
        exit(1);
      }
		/*       *userData = (void*) midistream; */
      /* empty the buffer after setting filter */
      while (Pm_Poll(midistream) == TRUE) {
        Pm_Read(midistream, &buffer, 1);
      }
#endif
    }
    {
      PmError     retval;
      PmDeviceInfo *info;
      PortMidiStream *midistream;

      /* check if there are any devices available */
		cnt_outdev = portMidi_getDeviceCount(1);
		if (cnt_outdev < 1) {
			/* return*/ printf("no output devices are available\n");
      }
		if (dev_midiout < 0 || dev_midiout >= cnt_outdev) {
        printf("error: device number is out of range\n");
        exit(1);
      }
      info = portMidi_getDeviceInfo(dev_midiout, 1);
      if (info->interf != NULL)
        printf("PortMIDI: selected output device %d: '%s' (%s)\n",
               dev_midiout, info->name, info->interf);
      else
        printf("PortMIDI: selected output device %d: '%s'\n",
               dev_midiout, info->name);
		
		outdev = dev_midiout + cnt_indev;
#ifdef NOTDEF
      retval = Pm_OpenOutput(&midistream,
                             (PmDeviceID) portMidi_getRealDeviceID(dev_midiout, 1),
                             NULL, 512L, (PmTimeProcPtr) NULL, NULL, 0L);
      if (retval != pmNoError) {
        printf("error opening output device %d: %s",
               dev_midiout, Pm_GetErrorText(retval));
        exit(1);
      }
#endif
      /*     *userData = (void*) midistream; */
    }

}

PmStream * Midiin=NULL, * Midiout=NULL;
void
midiopen(void)
{
    TIME_START;
    Pm_OpenInput(&Midiin, 
                /* 1,*/indev,
                 DRIVER_INFO, 
                 100, 
                 TIME_PROC, 
                 TIME_INFO);
    if (Midiin) {
      Pm_SetFilter(Midiin, PM_FILT_ACTIVE | PM_FILT_CLOCK);
      /* empty the buffer after setting filter, just in case anything
         got through */
      while (Pm_Poll(Midiin)) {
        PmEvent buffer;
        Pm_Read(Midiin, &buffer, 1);
      }
    }
    Pm_OpenOutput(&Midiout, 
                  /*2,*/outdev, 
                  DRIVER_INFO,
                  100, 
                  TIME_PROC, TIME_INFO, 
                  500); 
}

void
midiout(struct midi_event *m)
{
    PmEvent buffer;
    if (Midiout==NULL) return;
	/* no midi output available as yet */
    buffer.timestamp = TIME_PROC(TIME_INFO);
    buffer.message = Pm_Message(NOTEON_TYPE | m->channel,
                                m->note, m->velocity);
    Pm_Write(Midiout, &buffer, 1);
}

void
control_out(int channel, int data1, int data2)
{
    PmEvent buffer;
    if (Midiout==NULL) return;
	/* no midi output available as yet */
    buffer.timestamp = TIME_PROC(TIME_INFO);
    buffer.message = Pm_Message(PROGRAM_TYPE | channel,
                                   data1, data2);
    Pm_Write(Midiout, &buffer, 1);
}


int
midiin(struct statement *s, Cell *locals, void **dp)
{
    PmEvent buffer[1];
    PmError length, status;
    int note_received;
    struct  exprlist *data = s->el;
    if (Midiout==NULL) return 0;
    status = Pm_Poll(Midiin);
    if (status == TRUE) {
      length = Pm_Read(Midiin,buffer,1);
      if ((Pm_MessageStatus(buffer[0].message)&0xF0) == NOTEON_TYPE) {
        note_received = NOTE_EVENT;
        /* pass channel to tv input and get next data address */
        LGVAR(data->e,locals)->u.d =
          (double)(Pm_MessageStatus(buffer[0].message)&0x0F);
        data = data->next;
        /*pass note number to tv input and get next data address  */
        LGVAR(data->e,locals)->u.d =
          (double)Pm_MessageData1(buffer[0].message);
        data = data->next;
        /* pass velocity to tv input */
        LGVAR(data->e,locals)->u.d =
          (double)Pm_MessageData2(buffer[0].message);
      }
      /* Is Status byte an aftertouch event? */
      else if ((Pm_MessageStatus(buffer[0].message)&0xF0) == AFTOUCH_TYPE) {
        note_received = AFTERTOUCH;
        /* pass channel to tv input and get next data address */
        LGVAR(data->e,locals)->u.d =
          (double)(Pm_MessageStatus(buffer[0].message)&0x0f);
        data = data->next;
        /* pass amount to tv input */
        LGVAR(data->e,locals)->u.d =
          (double)Pm_MessageData1(buffer[0].message);
      }
      /* Is Status byte a Controller event? */
      else if((Pm_MessageStatus(buffer[0].message)&0xF0) == CONTROL_TYPE){	
        note_received = CONTROLLER;
        /* pass channel to tv input and get next data address*/
        LGVAR(data->e,locals)->u.d =
          (double)(Pm_MessageStatus(buffer[0].message)&0x0F);
        data = data->next;
        /* pass data byte1 to tv input and get next data address*/
        LGVAR(data->e,locals)->u.d =
          (double)Pm_MessageData1(buffer[0].message);
        data = data->next;
        /* pass data byte2 to tv input */
        LGVAR(data->e,locals)->u.d =
          (double)Pm_MessageData2(buffer[0].message);
      }
      /* If Status byte a note off, convert to note on velocity 0 */
      else if((Pm_MessageStatus(buffer[0].message)&0xF0) == NOTEOFF_TYPE) { 
          note_received = NOTE_EVENT;
          /* pass channel to tv input and get next data address*/
          LGVAR(data->e,locals)->u.d =
            (double)(Pm_MessageStatus(buffer[0].message)&0x0F);
          data = data->next;
          /* pass note number tv to input and get next data address */
          LGVAR(data->e,locals)->u.d =
            (double)Pm_MessageData1(buffer[0].message);
          data = data->next;
          /* pass velocity 0 to tv input */
          LGVAR(data->e,locals)->u.d = 0.0;
	}
      return note_received;
    }
    return NOTHING;
}

void
midiclose(void)
{
    int i;
    PmEvent buffer;
    if (Midiout) {
      for(i=0;i<16;i++) {
        buffer.timestamp = TIME_PROC(TIME_INFO);
        buffer.message = Pm_Message(0xB0 + i, 0x7B, 0);
        Pm_Write(Midiout, &buffer, 1);
      }
    }
    if (Midiout || Midiin) Pm_Terminate();
}


void
midiset(int channel, int instr)
{
    PmEvent buffer[5];
    buffer[0].timestamp = TIME_PROC(TIME_INFO);
    buffer[0].message = Pm_Message(PROGRAM_TYPE | channel, instr, 0);
    Pm_Write(Midiout, buffer, 1);
      /* Watch this space */
}

void
pitchbend(int channel, int bend)
{
	/* Ditto */
}

/*
 *	mouse support stub
 */
short mouse_x, mouse_y;

void
get_mouse_coords()
{
	mouse_x = 0;
	mouse_y = 0;
}
