#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef LATTICE
# include <conio.h>
# include <osbind.h>
# include <gemlib.h>
#endif
#ifdef __MACH__
# include <sys/malloc.h>
#else
#ifndef	LATTICE
# include <malloc.h>
#endif
#endif
#include <memory.h>
#include "tv.h"

#define	VERSION	"V1.5e: R.O./JPff Nov 2018"
	/* NB This version has improvements by John ffitch  -
	      #include,
	      ability to declare tables without size, etc...
		Encryption ability added November 2005.
	*/
FILES	*infiles;
char	ruleflnm[20];
char	formfile[20];	/* for ScoreBuilder */
int	immed = 0;
int	midas_used = 0;
struct	program *prog = 0;
int	yydebug;

void
usage(void)
{
#ifdef _WIN32
	fprintf(stderr, "\nUsage:\ttv [-I -iN -oN -v] rule_file");
#else
	fprintf(stderr, "Usage:\ttv [-i -v] rule_file");
#endif
/*	fprintf(stderr, "\n\twhere\t-d = dump global variables,");*/
	fprintf(stderr, "\n\t\t-i = immediate performance,");
/*	fprintf(stderr, "\n\t\t-p = show pass 1 parser state,");*/
	fprintf(stderr, "\n\t\t-v = verbose mode\n");
	fprintf(stderr,"\n\t\t-IN = use MIDI IN device N (N > 0)");
	fprintf(stderr,"\n\t\t-oN = use MIDI OUT device N (N > 0)\n");
	list_mididevs();

	exit(0);
}

int
main(int argc, char *argv[])
{
	/*RWD 4:2001 use 0 to signal MIDI_MAPPER. NB no MIDI_MAPPER for input !*/

	int midi_indev = 1, midi_outdev = /*1*/0;
	int cnt;
	unsigned int i;
	int pr_1 = 0;
	int verbose = 0;
	FILE *infile;
#ifdef LATTICE
	stderr->_file = open("con:", 2);
#endif
	if(argc > 1)
	{
		for(i=0; argv[i] != 0; i++) {
			if(strncmp(argv[i], "form.", 5) == 0) {
                        	 strncpy(formfile, argv[i], 20);
                          	formfile[19]='\0';
				printf("\nForm file is: %s\n", formfile);
			}
		}
		while(argv[1] != 0 && argv[1][0] == '-')
		{
			for(i = 0; i < (int) strlen(argv[1]); i++)
			{
				switch(argv[1][i])
				{
				case 'd':   dump_globals = TRUE;
					break;
				case 'h':   usage();
					break;
				case 'I':   immed = TRUE;	/* IMMEDIATE MODE */
					break;
				case 'p':   pr_1 = TRUE;
					break;
				case 'v':   verbose = TRUE;	/* DEBUG MODE */
					break;
				case 'y':   yydebug = 1;
					break;
				case 'o':
					if(argv[1][2]== '\0'){
						fprintf(stderr,"\n-o flag needs a device number");
						usage();
						break;
					}
					midi_outdev = atoi(&(argv[1][2]));
					if(midi_outdev < 1){
						fprintf(stderr,"\nMIDI Out device must be > 0");
						usage();
						break;
					}
					break;
				case 'i':
					if(argv[1][2]== '\0'){
						fprintf(stderr,"\n-I flag needs a device number");
						usage();
						break;
					}
					midi_indev = atoi(&(argv[1][2]));
					if(midi_indev <1){
						fprintf(stderr,"\nMIDI In device must be > 0");
						usage();
						break;
					}
					break;
				default:
					break;
				}
			}
			argc--; argv++;
		}
	}
	if(argc < 2) {
	    printf("TABULA VIGILANS: Version %s\n", VERSION);
	    usage();
	}

	infiles = (FILES*)malloc(sizeof(FILES));
	if((infile = fopen(argv[1], "r")) == 0) {
		strncpy(ruleflnm, argv[1],20);
		strncat(ruleflnm, ".tv", 20-strlen(ruleflnm));
		ruleflnm[19] = '\0';
		if((infile = fopen(ruleflnm, "r")) == NULL) {
		  strncat(ruleflnm, "s", 20-strlen(ruleflnm));
		  ruleflnm[19] = '\0';
		  if((infile = fopen(ruleflnm, "r")) == NULL) {
		    fprintf(stderr, "TV: Can't open file '%s' for rules input\n",
			    argv[1]);
		    exit(1);
		  }
		}
		infiles->name = ruleflnm;
	} else infiles->name = argv[1];
	infiles->fd = infile;
	{		/* Is this an encrypted file?? */
		int ch = getc(infiles->fd);
		infiles->encrypt = (ch=='\0');
		if (!infiles->encrypt) ungetc(ch, infiles->fd);
		else {
			fclose(infiles->fd);
			infiles->fd = fopen(infiles->name, "rb");
			getc(infiles->fd);
		}
	}
	infiles->next = NULL;
	prog_argc = argc - 2;
	prog_argv = argv + 2;

	inits(midi_indev -1,midi_outdev -1);
	initsymtab();
	printf("TABULA VIGILANS: Version %s\n", VERSION);

	#if YYDEBUG
		yydebug=1;
	#endif

	if(yyparse() != 0) {
		fprintf(stderr, "tv: processing failed owing to syntax errors\n");
		tidy_up(0);
	}
	if(pr_1)
		prprog(prog, stderr);
	if(verbose)
		printf("PASS2\n");
	if((cnt = pass2(prog)) > 0) {
		if(cnt == 1)
		    printf("There was one error\n");
		else
		    printf("There were %d errors\n", cnt);
		tidy_up(0);
	}
	if(verbose)
		prprog(prog, stderr);
	if(!immed) {
#ifdef ATARI
		fprintf(stderr,
			"Ready to run performance (Press Enter key: Any Key to end):\n");
		getch();
#else
		fprintf(stderr,
			"Ready to run performance (Press Enter key: CONTROL-C to end):\n");
		getchar();
#endif
	}
	else
#ifdef ATARI
		printf("Running... Any Key to end:\n");
#else
		printf("Running... CONTROL-C to end:\n");
#endif
	perf_time(0);
	execute(prog);
	tidy_up(1);
	return 0;
}
