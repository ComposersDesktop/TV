/* *** JOHN'S MIDI.H *** */

enum event_type {note_on, note_off};

struct midi_event {
  enum event_type type;
  TICK time;
  int channel;
  int note;
  int velocity;
  long *inuseptr;
  struct midi_event *next;
};

extern int midi_used;

#define NOTHING  0x00
#define NOTEOFF  0x80
#define NOTEON   0x90
#define POLYAFT  0xA0
#define CONTROL  0xB0
#define PROGRM   0xC0
#define AFTOUCH  0xD0
#define PCHBEND  0xE0
#define SYSTEM   0xF0

/* RWD from previous version */
#define NOTE_EVENT 1
#define PITCH_BEND 2
#define AFTERTOUCH 3
#define CONTROLLER 4

void control_out(int, int, int);
void midi_init(void);
void midi_event(enum event_type, TICK, int, int, int, long *);
void midi_poll(void);
void midi_cleanup(int);

void midi_note(TICK, TICK, int, int, int, long *);
#ifdef _WIN32
void cls(void);
TICK current_time();
void get_mouse_coords();
#endif
/*
 *	machine independent support routines
 */
void midiopen(void);
void midiclose(void);
void midiecho(struct midi_event *);
void midiout(struct midi_event *);

/*
 *	machine dependent routines
 */
void midiset(int, int);
void pitchbend(int, int);
int  midiin(struct statement *, struct cell *, void **);
