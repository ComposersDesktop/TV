/* win32.c: use this file to build for Windows95\NT under Visual C++
 * R.W.Dobson October 1996
 */
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <time.h>
#include <signal.h>
#include <io.h>
#ifdef _DEBUG
#include <assert.h>
#endif
#include "tv.h"
#include "rules.h"
//#define RORTON
#include "midi.h"

static void CALLBACK MidiProc(HMIDIIN Hin,UINT msg,DWORD inst,DWORD param1,DWORD param2);
static void clrscreen( HANDLE hConsole );


struct midi_in_data {
		int quality;
		BYTE status;
        BYTE data1;
		BYTE data2;        
    };

#define M_USED (0)
#define M_NEW (1)


//dreaded globals...
int finish;
short  mouse_button, mouse_x, mouse_y;
short  ConWidth,ConHeight;

static UINT MidiOutID = 0;
static UINT MidInID = 0;
static HMIDIOUT OutHandle = 0;
static HMIDIIN InHandle = 0;
static UINT MidiOutdev = /*0*/MIDI_MAPPER;
static UINT MidiIndev = 0;
static struct midi_in_data indata = {M_USED,0,0,0};

void
cls(void)
{
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	clrscreen(hStdout);			
	printf("\nTabula Vigilans:\n");
}

//straight from VC++ Knowledge Base!
// (minus all the PERR debug calls )
//NB need the #ifdef _WIN32 in execute.c
static void clrscreen( HANDLE hConsole )
{
   COORD coordScreen = { 0, 0 };    /* here's where we'll home the
                                       cursor */
   BOOL bSuccess;
   DWORD cCharsWritten;
   CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
   DWORD dwConSize;                 /* number of character cells in
                                       the current buffer */
 
   /* get the number of character cells in the current buffer */
 
   bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
   dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
 
   /* fill the entire screen with blanks */
 
   bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
      dwConSize, coordScreen, &cCharsWritten );
 
   /* get the current text attribute */
 
   bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
 
   /* now set the buffer's attributes accordingly */
 
   bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
      dwConSize, coordScreen, &cCharsWritten );
 
   /* put the cursor at (0, 0) */
 
   bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
   return;
}


TICK
current_time()
{       
   return (TICK) GetTickCount();
}


/*RWD: See Schildt p 344 for Console Mouse events*/
/* unfortunately, coords are in character mode */
/* this does not wait for a mouse event; if there is one, it is reported*/
void
get_mouse_coords()
{
    HANDLE hStdin;
    INPUT_RECORD inbuf;
    DWORD result = 0;
	DWORD console_mode;


    hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if(hStdin==INVALID_HANDLE_VALUE){
		fprintf(stderr,"Unable to read Mouse\n");
		return;
	}
	/* need to do this to work under NT/Win2K */
	GetConsoleMode(hStdin,&console_mode);
	SetConsoleMode(hStdin,console_mode  | ENABLE_MOUSE_INPUT); 

    ReadConsoleInput(hStdin,&inbuf,1,&result);
	
    if(result && inbuf.EventType==MOUSE_EVENT){
        mouse_x = (short) inbuf.Event.MouseEvent.dwMousePosition.X;
        mouse_y = (short) inbuf.Event.MouseEvent.dwMousePosition.Y;
        mouse_button = (short) inbuf.Event.MouseEvent.dwButtonState;

    }
	SetConsoleMode(hStdin,console_mode);
}



/* Midiin done differently in Windows:
 * callback func writes event, 
 * which is read by midiin()
 * NB for now, no filtering here of unwanted msgs, all done in midiin()
 */
/* one day, if more than one MIDI in device, must test which one...*/
static void CALLBACK MidiProc(HMIDIIN Hin,UINT msg,DWORD inst,DWORD param1,DWORD param2)
{
	 union {
        DWORD Ldata;
        BYTE data[4];
    } dat;

	switch(msg) {
	case(MIM_DATA):
		dat.Ldata = param1;
		indata.quality = M_NEW;
		indata.status =  dat.data[0];
		indata.data1  =  dat.data[1];
		indata.data2 =  dat.data[2];
		break;
#ifdef _DEBUG
	case(MIM_ERROR):
		break;
	case(MIM_CLOSE):
		break;
	case(MIM_OPEN):
		break;
	case(MIM_LONGDATA):
		break;
	case(MIM_LONGERROR):
		break;
#endif
	default:
		break;
	}	
}

/*  NB Windows MIDI IN ports always include status byte: no running status
 *	We get a complete message, not single bytes
 *	for now, only returns MSB of Continuous Controller msgs
 *	Also, midiin rule always returns two MIDI data bytes, so must set dummy
 *	second one for Channel Pressure
 */
int
midiin(struct statement *s, struct cell *locals, void **dp)
{
    struct exprlist *data = s->el;    
    BYTE status;
    int	note_received = NOTHING;
	//ignore all System Real_time msgs	
	if(indata.quality == M_USED || indata.status > 0xF7)		
		return note_received;

	//else it is a new event
 	status = indata.status;
			
	if((status&0xE0) == 0x80) {   /* Note Event */
			note_received = NOTE_EVENT;
			LGVAR(data->e,locals)->u.d = (double)(status&0x0F);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double) indata.data1;
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double) indata.data2;
	}
#ifndef RORTON
	else if((status&0xE0) == 0xE0){					/*Pitchbend: RWD*/
			short val = (((short)indata.data2)<<7) + (short)indata.data1;
			note_received = PITCH_BEND;
 			LGVAR(data->e,locals)->u.d = (double)(status&0x0F);
			data = data->next;
 			//LGVAR(data->e,locals)->u.d = (double) indata.data1;			
 			LGVAR(data->e,locals)->u.d = (double) indata.data2;	//lores bend			
			data = data->next;
			//LGVAR(data->e,locals)->u.d = (double) indata.data2;
			LGVAR(data->e,locals)->u.d = (double) val;		//hires bend			
	}
#endif
	/*RWD: Channel pressure: data2 undefined by R.O*/
	else if((status&0xF0) == 0xD0) {				/* Aftertouch */
			note_received = AFTERTOUCH;
			LGVAR(data->e,locals)->u.d = (double)(status&0x0F);
			data = data->next;
			LGVAR(data->e,locals)->u.d = (double) indata.data1;
			//RWD: now set a dummy second datum
			data = data->next;
			LGVAR(data->e,locals)->u.d = 0.0;
	}
	else if((status&0xF0) == 0xB0) {				/* Controller Info */
/*RWD: filter out any LSB controller msgs */
			if(indata.data1 > 0x1F && indata.data1 < 0x40)
				note_received = NOTHING;
			else {
				note_received = CONTROLLER;
				LGVAR(data->e,locals)->u.d = (double)(status&0x0F);
				data = data->next;
				LGVAR(data->e,locals)->u.d = (double) indata.data1;
				data = data->next;
				LGVAR(data->e,locals)->u.d = (double) indata.data2;
			}
	}    
    if(note_received != NOTHING) {  /* If event received */
			if((status&0xF0) == 0x80) { /* If 'twas a Note */
			//	data = data->next;
            //    data = data->next;
                LGVAR(data->e,locals)->u.d = (double)0.;
                /* Turn any note OFF's into velocity 0 */		
			}
    }
	//tell callback we've got it
	indata.quality = M_USED;
    return note_received;
}

/* RWD: new R.O code October 1996 */
void
perf_time(int report) /* stores time of performance */
{
	double secs;
	unsigned long mins =0L, hours = 0L, days = 0L;

	register unsigned long timevalue = current_time();
    static int    first = TRUE;
	static unsigned long	start_systime;


	if(first) {
	    start_systime = timevalue;
	    first = FALSE;
	}

	if(report) {		
#ifdef NOTDEF
		secs = (double)(timevalue - start_systime) /(double)TICKSPERSEC;
	    while(secs >= 60.0) {
			mins++;
			secs -= 60.0;
	    }
	    if(!mins) printf("\nPerformance Time: %.2lf\"", secs);
	    else printf("\nPerformance Time: %2ld' %.2lf\"", mins, secs);
#endif	
	    secs = (double)(timevalue - start_systime)/(double)TICKSPERSEC;
	    mins = (unsigned long)(0.5 + (long)(secs/60.0));
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
				else
					printf("\nPerformance Time: %3ld day, %2ld hrs. %2ld' %.2lf\"\n",
						days, hours, mins, secs);
			} 
			else {
				if(hours == 1)
					printf("\nPerformance Time: %3ld days, %2ld hr. %2ld' %.2lf\"\n",
						days, hours, mins, secs);
				else 
					printf("\nPerformance Time: %3ld days, %2ld hrs. %2ld' %.2lf\"\n",
						days, hours, mins, secs);
		    }
		}
		else 
		if(hours) {
		    if(hours == 1)
				printf("\nPerformance Time: %2ld hr. %2ld' %.2lf\"\n",
					hours, mins, secs);
		    else 
				printf("\nPerformance Time: %2ld hrs. %2ld' %.2lf\"\n",
					hours, mins, secs);
		}
		else 
		if(mins)
		    printf("\nPerformance Time: %2ld' %.2lf\"\n",
				mins, secs);
		else
		    printf("\nPerformance Time: %.2lf\"\n", secs);
	}
}

//stuff from unix.c
static void
hdlr(int sig)
{
	finish = 1;
	signal(SIGINT, hdlr);
}
//RWD.11.98
void list_mididevs(void)
{
	UINT devs;

	devs = midiInGetNumDevs();
	if(devs==0){
		fprintf(stderr,"\nNo MIDI input devices found");
	}
	else {
		UINT thisdev;
		MIDIINCAPS incaps;
		printf("\nfound %d MIDI IN devices:",devs);
		for(thisdev=0; thisdev < devs;thisdev++){
			if(midiInGetDevCaps(thisdev,&incaps,sizeof(MIDIINCAPS)) == MMSYSERR_NOERROR)
				printf("\n  %d:   %s",thisdev+1,incaps.szPname);
			else
				fprintf(stderr,"\n  (%d: error reading device information)",thisdev+1);
				
		}
	}
	devs = midiOutGetNumDevs();
	if(devs==0){
		fprintf(stderr,"\nNo MIDI output devices found");
	}
	else {
		UINT thisdev;
		MIDIOUTCAPS outcaps;
		printf("\nfound %d MIDI OUT devices:",devs);
		for(thisdev=0; thisdev < devs;thisdev++){
			if(midiOutGetDevCaps(thisdev,&outcaps,sizeof(MIDIOUTCAPS)) == MMSYSERR_NOERROR)
				printf("\n  %d:   %s",thisdev+1,outcaps.szPname);
			else
				fprintf(stderr,"\n  (%d: error reading device information)",thisdev+1);				
		}
	}
}

/*RWD: NB: Boo Hiss, no error returns from these funcs, yet... */
//RWD.11.98 full selection of In and OUT devices
void
inits(int dev_midiin,int dev_midiout)
{
    HANDLE hStdin,hStdout;
    CONSOLE_CURSOR_INFO cinfo;
	COORD ConSize;
	UINT dev;
	MIDIINCAPS	incaps;
    MIDIOUTCAPS outcaps;
	int found = 0;


	/*RWD 3:2001 try to use MIDI_MAPPER? */
	if(dev_midiout==0)
		dev_midiout = MIDI_MAPPER;
	if(dev_midiin==0)
		dev_midiin = MIDI_MAPPER;

	finish = 0;
	if(signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, hdlr);

	dev = midiInGetNumDevs();
	if(dev==0){
		fprintf(stderr,"\nNo MIDI IN device found");
	}
	else{
		MidiIndev = (UINT) dev_midiin;		//RWD.11.98
		if(
		   (midiInGetDevCaps(MidiIndev,&incaps,sizeof(MIDIINCAPS))== MMSYSERR_NOERROR)
		   &&										
		   (midiInOpen(&InHandle,MidiIndev,(DWORD)MidiProc,1,CALLBACK_FUNCTION)== MMSYSERR_NOERROR)
		   &&
		   (midiInStart(InHandle)== MMSYSERR_NOERROR)
			) {
				printf("\nUsing %s for MIDI input",incaps.szPname);					
		}
		else{
		 	   fprintf(stderr,"\nWARNING: error initializing  MIDI input");
		}
	}

    //MIDI OUT	
	dev = midiOutGetNumDevs();
    if(dev < 1) {
        fprintf(stderr,"\nNo MIDI Output device - exiting");
        exit(1);
    }
	else {
		MidiOutdev =dev_midiout;
			
		if(midiOutGetDevCaps(MidiOutdev,&outcaps,sizeof(MIDIOUTCAPS))!= MMSYSERR_NOERROR){
			fprintf(stderr,"\ntv: error querying device %ld",MidiOutdev+1);
			return;
		}		
		printf("\nUsing %s for output\n",outcaps.szPname);		
	}

	//MIDI OK: now console stuff: hide cursor and clear any lingering input
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdout,&cinfo);
    cinfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdout,&cinfo);    
    //get size of console for use with mouse function
	ConSize = GetLargestConsoleWindowSize(hStdout);
	ConWidth = ConSize.X;		 //get odd widths here!
	ConHeight = ConSize.Y;
	//correct ConWidth for allowed widths in MSDOS:
	if(ConWidth < 80) 
		ConWidth = 40;
	if(ConWidth > 80)
		ConWidth = 80;

#ifdef _DEBUG	
	assert(ConWidth > 0 && ConHeight > 0);	
#endif	
	FlushConsoleInputBuffer(hStdin);
    //no srand48() equiv in win32, so just use srand
    srand(GetTickCount());
}

void
mach_cleanup()
{
    HANDLE hStdin,hStdout;
    CONSOLE_CURSOR_INFO cinfo;

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdout,&cinfo);
    cinfo.bVisible = TRUE;
    SetConsoleCursorInfo(hStdout,&cinfo);
}

/*
 *	midi support routines
 */
void
control_out(int channel, int data1, int data2)
{
    union {
        DWORD Ldata;
        BYTE data[4];
    } dat;

    (dat.data)[0] = 0xB0 + (BYTE)channel;
    (dat.data)[1] = (BYTE)data1;
    (dat.data)[2] = (BYTE)data2;
    (dat.data)[3] =  (BYTE)0;
    midiOutShortMsg(OutHandle,dat.Ldata);
}


void
midiopen()
{
    int result;

    result = midiOutOpen(&OutHandle,MidiOutdev,0,0,CALLBACK_NULL);

    if(result != MMSYSERR_NOERROR){
        if(result==MMSYSERR_ALLOCATED){
            fprintf(stderr,"\nSorry, MIDI output device already in use");
        }
        else
            fprintf(stderr,"\nError opening MIDI output device");
        exit(1);
    }

}

void
midiclose()
{
    	midiInReset(InHandle);
	midiInClose(InHandle);
	if(midi_used){
		midiOutReset(OutHandle);
		if(midiOutClose(OutHandle)!= MMSYSERR_NOERROR)
			fprintf(stderr,"\nWARNING: problem closing MIDI Output device");
	}
}

void
midiout(struct midi_event *m)
{
    union {
        DWORD Ldata;
        BYTE data[4];
    } dat;


    (dat.data)[0] = (BYTE) (0x90 + m->channel);
    (dat.data)[1] = (BYTE) m->note;
    (dat.data)[2] = (BYTE) m->velocity;
    (dat.data)[3] = (BYTE) 0;
    midiOutShortMsg(OutHandle,dat.Ldata);
}

void
midiset(int channel, int instr)
{
    union {
        DWORD Ldata;
        BYTE data[4];
    } dat;

    (dat.data)[0] = (BYTE) (0xc0 + channel);
    (dat.data)[1] = (BYTE) instr;
    (dat.data)[2] = (BYTE) 0;
    (dat.data)[3] = (BYTE) 0;
    midiOutShortMsg(OutHandle,dat.Ldata);
}

void
pitchbend(int channel, int bend)
{
    union {
        DWORD Ldata;
        BYTE data[4];
    } dat;

    (dat.data)[0] = (BYTE) (0xE0 + channel);
    (dat.data)[1] = (BYTE) 0;
    (dat.data)[2] = (BYTE) bend;
    (dat.data)[3] = (BYTE) 0;
    midiOutShortMsg(OutHandle,dat.Ldata);
}
