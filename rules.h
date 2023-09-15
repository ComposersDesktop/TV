/*
 *      Tabula Vigilans
 *         "RULES.H"
 *  Rule execution functions Header File
 */

#define check_lock(expr, locals)                    \
            (((expr)->op == '[')                \
                ? 0                 \
                : LGVAR((expr),(locals))->lock)

#define isatable(e) ((e)->op == TIDENT || (e)->op == '[')

typedef unsigned long TICK;

#ifdef _WIN32
    #define TICKSPERSEC (1000)  //PC
# else
    #define TICKSPERSEC (200)   // Mac/linux using PortMidi
#endif

#define SCREENWIDTH 639 /* ST High Resolution */
#define SCREENHEIGHT    399

extern  short   mouse_x, mouse_y;
extern  Sfile   *sfile;         /* Storage file descriptors */
extern  int     decdig;
extern  int sndoverflows;

TICK current_time(void);
void get_mouse_coords(void);
void rule_err(struct proc *, char *, char *);
double taborval(struct expr *, int, struct cell *);

extern struct rule r_sfread;

typedef int t_ruleexec(Statement *, struct cell *, void **);
typedef int t_ruleinit(void **);
typedef int t_rulecheck(Statement *, struct proc *, Program *);
#define RULEDECL(rulename)  t_ruleexec e_##rulename;  \
                t_rulecheck c_##rulename

RULEDECL(init);
RULEDECL(table);

RULEDECL(add_dec);
RULEDECL(alllocked);
RULEDECL(anylocked);
RULEDECL(compare);  t_ruleinit i_compare;
RULEDECL(close_storefiles);
RULEDECL(control_out);
RULEDECL(copy);
RULEDECL(copy_table);
RULEDECL(embed);    t_ruleinit i_embed;
RULEDECL(fail);
RULEDECL(fold);     t_ruleinit i_fold;
RULEDECL(generate); t_ruleinit i_generate;
RULEDECL(interp_table);
RULEDECL(lim);
RULEDECL(linexplog);    t_ruleinit i_linexplog;
RULEDECL(lock);
RULEDECL(max);
RULEDECL(lintrans);
RULEDECL(mean);
RULEDECL(message);      t_ruleexec e_messageq;
RULEDECL(messag1);  t_ruleinit i_message;
RULEDECL(midichord);    t_ruleinit i_midichord;
RULEDECL(midiecho); t_ruleinit i_midiecho;
RULEDECL(midiin);   t_ruleinit i_midiin;
RULEDECL(midiout);  t_ruleinit i_midiout;
RULEDECL(midiset);
RULEDECL(min);
RULEDECL(mouse);
RULEDECL(mult);
RULEDECL(mult_table);
RULEDECL(offset_table);
RULEDECL(perm);     t_ruleinit i_perm;
RULEDECL(pitchbend);
RULEDECL(pop);
RULEDECL(print);
RULEDECL(probe);
RULEDECL(probi);
RULEDECL(scale_table);
RULEDECL(schedule); t_ruleinit i_schedule;
RULEDECL(seg);      t_ruleinit i_seg;
RULEDECL(shift);
RULEDECL(sort);
RULEDECL(store);
RULEDECL(stori);
RULEDECL(subst);
RULEDECL(sum);
RULEDECL(sum_table);
RULEDECL(swap);
RULEDECL(time);     t_ruleinit i_time;
RULEDECL(trigger);  t_ruleinit i_trigger;
RULEDECL(unlock);
RULEDECL(wait);     t_ruleinit i_wait;
RULEDECL(xadr);     t_ruleinit i_xadr;
RULEDECL(send);
/*RULEDECL(sfcreate);
RULEDECL(sfopen);
RULEDECL(putprop);
RULEDECL(sfrw);
RULEDECL(sfclose);*/

double mf_abs(double);
double mf_argc(void);
double mf_argv(double);
char * mf_args(double);
double mf_dimensions(struct table *);
double mf_dimsize(struct table *, double);
double mf_gamma(void);
double mf_gauss(void);
double mf_power(double, double);
double mf_rand(void);
double mf_random(double, double);
double mf_round(double);
double mf_sine(double);
double mf_cosine(double);
double mf_tangent(double);
double mf_sqrt(double);
double mf_arcsine(double);
double mf_arccosine(double);
double mf_arctangent(double);
double mf_natlog(double);
double mf_log10(double);



