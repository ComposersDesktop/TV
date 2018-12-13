/*
 *	configuration tables for tv
 */
#include <stdio.h>
#include <math.h>
#include "tv.h"
#include "rules.h"
#include "midi.h"
#include "xtab.h"

static struct mfunc f_abs  =       { "abs",		1,0,	mf_abs };
static struct mfunc f_argc =       { "argc",		0,0,	mf_argc };
static struct mfunc f_argv =	   { "arg",		1,0,	mf_argv };
static struct mfunc f_dimensions = { "dimensions",	1,1,	mf_dimensions };
static struct mfunc f_dimsize =	   { "dimsize",		2,1,	mf_dimsize };
static struct mfunc f_int =	   { "int",		1,0,	floor };
static struct mfunc f_gamma =	   { "gamma",		0,0,	mf_gamma };
static struct mfunc f_gauss =	   { "gauss",		0,0,	mf_gauss };
static struct mfunc f_power =	   { "power",		2,0,	mf_power };
static struct mfunc f_rand =	   { "rand",		0,0,	mf_rand };
static struct mfunc f_random =	   { "random",		2,0,	mf_random };
static struct mfunc f_round =	   { "round",		1,0,	mf_round };
static struct mfunc f_sine  =	   { "sine",		1,0,	mf_sine };
static struct mfunc f_sqrt  =	   { "sqrt",		1,0,	mf_sqrt };
static struct mfunc f_cosine  =	   { "cosine",		1,0,	mf_cosine };
static struct mfunc f_tangent  =   { "tangent",		1,0,	mf_tangent };
static struct mfunc f_arcsine  =   { "arcsine",		1,0,	mf_arcsine };
static struct mfunc f_arccosine  = { "arccosine",	1,0,	mf_arccosine };
static struct mfunc f_arctangent = { "arctangent",	1,0,	mf_arctangent };
static struct mfunc f_natlog =     { "natlog",          1,0,    mf_natlog };
static struct mfunc f_log10 =	   { "log10",		1,0,	mf_log10  };

#define ANY (-1)

/*					  TOT, LEFT, RIGHT,		*/
static Rule r_add_dec = {"add_dec",  4,   1,    3, 0, e_add_dec,0,c_add_dec	};
static Rule r_alllocked = {"alllocked", ANY,0,ANY, 0, e_alllocked,  0, c_lock		};
static Rule r_anylocked = {"anylocked", ANY,0,ANY, 0, e_anylocked,  0, c_lock		};
static Rule r_compare = { "compare", ANY, 1, ANY, 0, e_compare, i_compare, c_compare };
static Rule r_control_out = { "control_out", 3, 0, 3, 0, e_control_out, 0, c_control_out };
       Rule r_copy =   {	"copy",	  ANY, ANY,    1, 0, e_copy,  0,	c_copy	};
static Rule r_copy_table =  { "copy_table", 2, 1, 1, 0, e_copy_table,  0,	c_copy_table	};
static Rule r_embed = { "embed",3, 1, 2, 0, e_embed, i_embed, c_embed	};
static Rule r_exp =    {	"exp",	  ANY,   1,  ANY, 0, e_linexplog, i_linexplog, c_linexplog };
static Rule r_fail =   {	"fail",   ANY,   0,    0, 0, e_fail,  0,	c_fail	};
static Rule r_fold = { "fold",3, 1, 2, 0, e_fold, i_fold, c_fold	};
static Rule r_generate = { "generate",  2, 1, 1, 0, e_generate, i_generate, c_generate };
static Rule r_interp_table = { "interp_table",  4, 1, 3, 0, e_interp_table,   0,	c_interp_table	};
static Rule r_lim =    {	"lim",	  ANY, ANY,    2, 0, e_lim,   0,	c_lim	};
static Rule r_lin =    {	"lin",	  ANY,   1,  ANY, 0, e_linexplog, i_linexplog,	c_linexplog };
static Rule r_lintrans = {"lintrans",4,   1,    3, 0, e_lintrans, 0,c_lintrans	};
static Rule r_lock =   { "lock",   ANY,   0,  ANY, 0, e_lock,  0,   c_lock		};
static Rule r_log =    {	"log",	  ANY,   1,  ANY, 0, e_linexplog, i_linexplog, c_linexplog };
static Rule r_max =    {	"max",	  ANY,   1,  ANY, 0, e_max,   0,	c_max	};
static Rule r_mean =    { "mean",  ANY,   1,  ANY, 0, e_mean,   0,	c_mean	};
static Rule r_message = { "message", 1,  0,  1, 1, e_message, 0, c_message	};
static Rule r_messageq = { "messageq", 1,  0,  1, 1, e_messageq, 0, c_message	};
static Rule r_messag1 = { "messag1", 1,  0,  1, 1, e_message, i_message, c_message	};
static Rule r_min =    {	"min",	  ANY,   1,  ANY, 0, e_min,   0,	c_min	};
static Rule r_midichord = { "midichord", ANY, 0, ANY, 0, e_midichord, i_midichord, c_midichord	};
static Rule r_midiecho = { "midiecho", 3, 0,    3, 0, e_midiecho, i_midiecho, c_midiecho	};
static Rule r_midiin = { "midiin",   3,   3,    0, 0, e_midiin, 0, c_midiin	};
static Rule r_midiout = { "midiout",ANY,  0,  ANY, 0, e_midiout, i_midiout, c_midiout	};
static Rule r_midiset = { "midiset", 2,   0,    2, 0, e_midiset, 0, c_midiset	};
static Rule r_mouse =    { "mouse",ANY,   2,  ANY, 0, e_mouse,   0,	c_mouse	};
static Rule r_mult =    { "mult",  ANY,   1,  ANY, 0, e_mult,   0,	c_mult	};
static Rule r_mult_table = { "mult_table", 3, 1, 2, 0, e_mult_table,   0,	c_mult_table	};
static Rule r_offset_table = { "offset_table", 3, 1, 2, 0, e_offset_table,   0,	c_offset_table	};
static Rule r_perm =    { "perm",    ANY,   0,    ANY, 0, e_perm,  0, c_perm	};
static Rule r_pitchbend = {"pitchbend", 2, 0,   2, 0, e_pitchbend, 0, c_pitchbend	};
static Rule r_pop =    {	"pop",	    2,   1,    1, 0, e_pop,   0,	c_pop	};
static Rule r_print =  {	"print",  ANY,   0,  ANY, 0, e_print, 0,	c_print	};
static Rule r_probe =  {	"probe",  ANY,   0,  ANY, 0, e_probe, 0,	c_probe	};
static Rule r_probi =  {	"probi",  ANY,   0,  ANY, 0, e_probi, 0,	c_probi	};
static Rule r_schedule = {"schedule",5,   0,    5, 0, e_schedule, i_schedule, c_schedule	};
static Rule r_scale_table = { "scale_table", 3, 1, 2, 0, e_scale_table,   0,	c_scale_table	};
static Rule r_seg =    {	"seg",	  ANY,   1,  ANY, 0, e_seg, i_seg, c_seg };
static Rule r_shift =  {	"shift",  ANY,   0,  ANY, 0, e_shift, 0, c_shift	};
static Rule r_sort =   {	"sort",	  ANY,   0,  ANY, 0, e_sort,  0, c_sort	};
static Rule r_store =  {	"store",  ANY,   0,  ANY, 0, e_store, 0, c_store };
static Rule r_stor4 =  {	"stor4",  ANY,   0,  ANY, 0, e_store, 0, c_store };
static Rule r_stor6 =  {	"stor6",  ANY,   0,  ANY, 0, e_store, 0, c_store };
static Rule r_stori =  {	"stori",  ANY,   0,  ANY, 0, e_store, 0, c_store };
static Rule r_storf =  {	"storf",  ANY,   0,  ANY, 0, e_store, 0, c_store };
static Rule r_subst =  {	"subst",    4,   1,    3, 0, e_subst, 0, c_subst };
static Rule r_sum =    {	"sum", 	  ANY,   1,  ANY, 0, e_sum,   0, c_sum	};
static Rule r_sum_table = { "sum_table",  3,   1, 2, 0, e_sum_table,   0,	c_sum_table	};
static Rule r_swap =    { "swap",  2, 0,  2, 0, e_swap,   0, c_swap	};
static Rule r_time =   {	"time",	  ANY,   1,  ANY, 0, e_time, i_time, c_time	};
static Rule r_trigger =  { "trigger", 2,1,1, 0, e_trigger, i_trigger, c_trigger	};
static Rule r_unlock = { "unlock", ANY,   0,  ANY, 0, e_unlock,  0, c_lock		};
static Rule r_wait =   {	"wait",	    1,   0,    1, 0, e_wait, i_wait, c_wait	};
static Rule r_xad =    { "xad", 2, 1, 1, 0, e_xadr, i_xadr,	c_xadr	};
static Rule r_xar =    { "xar", 2, 1, 1, 0, e_xadr, i_xadr,	c_xadr	};
/*static Rule r_sfcreate = { "sfcreate", ANY,  1,  ANY, 1, e_sfcreate, 0, c_sfcreate	};
static Rule r_sfopen = { "sfopen", ANY,  1,  ANY, 1, e_sfopen, 0, c_sfopen	};
static Rule r_putprop = { "putprop", 3,  1, 2, 1, e_putprop, 0, c_putprop	};
static Rule r_sfwrite = { "sfwrite", ANY,  0,  ANY, 0, e_sfrw, 0, c_sfrw	};
       Rule r_sfread = { "sfread", ANY,  0,  ANY, 0, e_sfrw, 0, c_sfrw	};
static Rule r_sfclose = { "sfclose", 1,  0,  1, 0, e_sfclose, 0, c_sfclose	};
*//*				  	TOT, LEFT, RIGHT,	*/

struct keytab keytab[] = {
	{	"break",	BREAK,		0,	0	},
	{	"call", 	CALL,		0,	0	},
	{	"case",		CASE,		0,	0	},
	{	"cls",	 	CLS,		0,	0	},
	{	"continue",	CONTINUE,	0,	0	},
	{	"store_digits", DECDIG,		0,	0	},
	{	"default",	DEFAULT,	0,	0	},
	{	"else", 	ELSE,		0,	0	},
	{	"end", 		END,		0,	0	},
	{	"fill_table",	FILLTABLE,	0,	0	},
	{	"for",		FOR,		0,	0	},
	{	"if", 		IF,		0,	0	},
	{	"int2string",	INT2STRING,	0,	0	},
	{	"local", 	LOCAL,		0,	0	},
	{	"loop", 	LOOP,		0,	0	},
	{	"num2string",	NUM2STRING,	0,	0	},
	{	"return", 	RETURN,		0,	0	},
	{	"oscsend",	SEND,		0,	0	},
	{	"storefile",	STOREFILE,	0,	0	},
	{	"storstr",	STORSTR,	0,	0	},
	{	"switchon",	SWITCH,		0,	0	},
	{	"system",	OPSYSCALL,	0,	0	},
	{	"table",	TABLE,		0,	0	},
	{	"try",		TRY,		0,	0	},
	{	"while",	WHILE,		0,	0	},

	{	"add_dec", 	0,		&r_add_dec,	0	},
	{	"alllocked",	0,		&r_alllocked,	0	},
	{	"anylocked",	0,		&r_anylocked,	0	},
	{	"compare", 	0,		&r_compare,	0	},
	{	"control_out", 	0,		&r_control_out,	0	},
	{	"copy", 	0,		&r_copy,	0	},
	{	"copy_table", 	0,		&r_copy_table,	0	},
	{	"embed",	0,		&r_embed,	0	},
	{	"exp",	 	0,		&r_exp,		0	},
	{	"fail", 	0,		&r_fail,	0	},
	{	"fold",		0,		&r_fold,	0	},
	{	"generate",	0,		&r_generate,	0	},
	{	"interp_table",	0,		&r_interp_table,0	},
	{	"lim",	 	0,		&r_lim,		0	},
	{	"lin",	 	0,		&r_lin,		0	},
	{	"lintrans", 	0,		&r_lintrans,	0	},
	{	"lock",		0,		&r_lock,	0	},
	{	"log",	 	0,		&r_log,		0	},
	{	"max",	 	0,		&r_max,		0	},
	{	"mean",	 	0,		&r_mean,	0	},
	{	"message",	0,		&r_message,	0	},
	{	"messageq",	0,		&r_messageq,	0	},
	{	"messag1",	0,		&r_messag1,	0	},
	{	"min",	 	0,		&r_min,		0	},
	{	"midichord", 	0,		&r_midichord,	0	},
	{	"midiecho", 	0,		&r_midiecho,	0	},
	{	"midiin", 	0,		&r_midiin,	0	},
	{	"midiout", 	0,		&r_midiout,	0	},
	{	"midiset", 	0,		&r_midiset,	0	},
	{	"mouse",	0,		&r_mouse,	0	},
	{	"mult", 	0,		&r_mult,	0	},
	{	"mult_table",	0,		&r_mult_table,	0	},
	{	"offset_table",	0,		&r_offset_table,0	},
	{	"perm",	 	0,		&r_perm,	0	},
	{	"pitchbend", 	0,		&r_pitchbend,	0	},
	{	"pop",	 	0,		&r_pop,		0	},
	{	"print", 	0,		&r_print,	0	},
	{	"probe", 	0,		&r_probe,	0	},
	{	"probi", 	0,		&r_probi,	0	},
	{	"scale_table",	0,		&r_scale_table, 0	},
	{	"schedule", 	0,		&r_schedule,	0	},
	{	"seg",	 	0,		&r_seg,		0	},
	{	"shift", 	0,		&r_shift,	0	},
	{	"sort", 	0,		&r_sort,	0	},
	{	"store", 	0,		&r_store,	0	},
	{	"stor4", 	0,		&r_stor4,	0	},
	{	"stor6", 	0,		&r_stor6,	0	},
	{	"stori", 	0,		&r_stori,	0	},
	{	"storf", 	0,		&r_storf,	0	},
	{	"subst",	0,		&r_subst,	0	},
	{	"sum", 		0,		&r_sum,		0	},
	{	"sum_table",	0,		&r_sum_table,	0	},
	{	"swap",		0,		&r_swap,	0	},
	{	"time",		0,		&r_time,	0	},
	{	"trigger",	0,		&r_trigger,	0	},
	{	"unlock",	0,		&r_unlock,	0	},
	{	"wait",		0,		&r_wait,	0	},
	{	"xad", 		0,		&r_xad,		0	},
	{	"xar", 		0,		&r_xar,		0	},
/*	{	"sfcreate",	0,		&r_sfcreate,	0	},
	{	"sfopen",	0,		&r_sfopen,	0	},
	{	"putprop",	0,		&r_putprop,	0	},
	{	"sfwrite",	0,		&r_sfwrite,	0	},
	{	"sfread",	0,		&r_sfread,	0	},
	{	"sfclose",	0,		&r_sfclose,	0	},
*/
	{	"abs",		0,		0,	&f_abs		},
	{	"argc",		0,		0,	&f_argc		},
	{	"arg",		0,		0,	&f_argv		},
	{	"args",		ARGS,		0,	NULL		},
	{	"dimensions",	0,		0,	&f_dimensions	},
	{	"dimsize",	0,		0,	&f_dimsize	},
	{	"gamma",	0,		0,	&f_gamma	},
	{	"gauss",	0,		0,	&f_gauss	},
	{	"int",		0,		0,	&f_int		},
	{	"power",	0,		0,	&f_power	},
	{	"rand",		0,		0,	&f_rand		},
	{	"random",	0,		0,	&f_random	},
	{	"round",	0,		0,	&f_round	},
	{	"sine",		0,		0,	&f_sine		},
	{	"cosine",	0,		0,	&f_cosine	},
	{	"tangent",	0,		0,	&f_tangent	},
	{	"sqrt",		0,		0,	&f_sqrt		},
	{	"arcsine",	0,		0,	&f_arcsine	},
	{	"arccosine",    0,		0,	&f_arccosine	},
	{	"arctangent",   0,	        0,	&f_arctangent	},
	{       "natlog",       0,              0,      &f_natlog       },
	{       "log10",	0,		0,	&f_log10	},

	{	0, 0, 0, 0	}
};

