// New Version 1.4, combining source of V1.36b with best of V1.38g
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __MACH__
# include <sys/malloc.h>
#else
# include <malloc.h>
#endif
#include <memory.h>
#include "tv.h"

#define	VERSION	"V1.5f: R.O./JPff Nov 2018"
	/* NB This version has improvements by John ffitch  -
	      #include,
	      ability to declare tables without size, etc...
		Encryption ability added November 2005.
		"system" call added Aug 2007
		"spawn" call - RO added Oct 2007 but removed by JPff
          1.37  - spawn has malloced input args deprecated
          Dynamic memory allocation for ruleflnm, formfile, and strtbl.
          New rule 'close_storefiles' which can be invoked at any stage in a script.
          New keyword 'showargs(n)' which reproduces commandline arguments inside a script.
          1.38e has revised the exitcodes in exit, tidy_up and perf_time routines. R.O., October 2008.
          1.38f now has the capability to process encrypted filltable files. R.O. Nov 2008.
	*/
FILES	*infiles;
char	ruleflnm[20];
char	formfile[20];	/* for ScoreBuilder */
char    strtbl[20];     /* string table of filenames */
int	immed = 0, midiin_used = 0;
int	midas_used = 0;
struct	program *prog = 0;
int	yydebug;

void
usage(int lm)
{
#ifdef _WIN32
	fprintf(stderr, "\nUsage:\ttv [-I -iN -oN -v] rule_file");
#else
	fprintf(stderr, "Usage:\ttv [-i -v] rule_file");
#endif
/*	fprintf(stderr, "\n\twhere\t-d = dump global variables,");*/
	fprintf(stderr, "\n\t\t-I = immediate performance,");
/*	fprintf(stderr, "\n\t\t-p = show pass 1 parser state,");*/
	fprintf(stderr, "\n\t\t-v = verbose mode\n");
	fprintf(stderr,"\n\t\t-iN = use MIDI IN device N (N > 0)");
	fprintf(stderr,"\n\t\t-oN = use MIDI OUT device N (N > 0)\n");
	list_mididevs();

    if(lm) list_mididevs();

	exit(1);
}

int
main(int argc, char *argv[])
{
	/*RWD 4:2001 use 0 to signal MIDI_MAPPER. NB no MIDI_MAPPER for input !*/

	int midi_indev = 1, midi_outdev = /*1*/0, listmidi = 0;
	int cnt;
	unsigned int i;
	int pr_1 = 0;
	int verbose = 0;
    int len;
    FILE *infile, *strfile;
    char *ruleflnm;
	if(argc > 1)
	{
		for(i=0; i < argc; i++) {

                        char *pt;
			if(strncmp(argv[i], "form.", 5) == 0) {
				 strncpy(formfile, argv[i], 20);
				formfile[19]='\0';
				printf("\nForm file is: %s\n", formfile);
			}
			//char *pt;
			pt = strpbrk(argv[i], ".");
			if(pt!=NULL && strcmp(pt, ".tab")==0) {
			    char *strtbl = malloc(strlen(argv[i]) * sizeof(char)+1);
			    strcpy(strtbl, argv[i]);
			   	if((strfile = fopen(strtbl, "r")) == 0) {
			        fprintf(stderr, "Can't open file %s\n", strtbl);
			        exit(1);
			    } else {
					char s[MAXSTRING];
			        char c[MAXSTRING];
			        char *ss[MAXTABNAMES];
			        int i, j;
			        static int g=0, acnt = 0, cnt=0;
			        // How many files?
			        while(!feof(strfile)) {
			           if(fgets(s, MAXSTRING, strfile)) {
			               if(strncmp(s, "//", 2) == 0) {
			                   continue;
			               }
			               cnt += sscanf(s, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			                                 c,c,c,c,c,c,c,c,c,c,c,c,c,c,c,c,c,c,c);
					   }
			        }
			        if(cnt > MAXTABNAMES) {
			            fprintf(stderr, "Error: Maximum number of files in a string-table is 10!");
			            exit(1);
			        }
			        printf("\nThere are %d files in input string-table '%s'.\n", cnt, strtbl);
			        rewind(strfile);
			        printf("Contents:\n");
			        while(!feof(strfile)) {
			            if(fgets(s, MAXSTRING, strfile)) {
			                if(strncmp(s, "//", 2) == 0) {
			                    continue;
			                }
				    	}
					    if(g>=cnt) break;
//	                    printf("\nName %s len %d", s, strlen(s));
                        // How many files on this line?
                        acnt += sscanf(s, "%s%s%s%s%s%s%s%s%s%s", c,c,c,c,c,c,c,c,c,c);
//                      printf("Count on this line: %d", acnt);
                        // Find lengths of string-filenames & allocate memory.
                        sscanf(s, "%s", c); j = strlen(c);
/* 0 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s", ss[g]);
                        printf("\t%s\n", ss[g++]);
                        if(g>=acnt) {strcpy(c, ""); continue;}
                        sscanf(s, "%s%s", c,c); j = strlen(c);
/* 1 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s%s", c, ss[g]);
                        printf("\t%s\n", ss[g++]);
                        if(g>=acnt) {strcpy(c, ""); continue;}
                        sscanf(s, "%s%s%s", c,c,c); j = strlen(c);
/* 2 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s%s%s", c,c, ss[g]);
                        printf("\t%s\n", ss[g++]);
                        if(g>=acnt) {strcpy(c, ""); continue;}
                        sscanf(s, "%s%s%s%s", c,c,c,c); j = strlen(c);
/* 3 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s%s%s%s", c,c,c, ss[g]);
                        printf("\t%s\n", ss[g++]);
                        if(g>=acnt) {strcpy(c, ""); continue;}
                        sscanf(s, "%s%s%s%s%s", c,c,c,c,c); j = strlen(c);
/* 4 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s%s%s%s%s", c,c,c,c, ss[g]);
                        printf("\t%s\n", ss[g++]);
                        if(g>=acnt) {strcpy(c, ""); continue;}
                        sscanf(s, "%s%s%s%s%s%s", c,c,c,c,c,c); j = strlen(c);
/* 5 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s%s%s%s%s%s", c,c,c,c,c, ss[g]);
                        printf("\t%s\n", ss[g++]);
                        if(g>=acnt) {strcpy(c, ""); continue;}
                        sscanf(s, "%s%s%s%s%s%s%s", c,c,c,c,c,c,c); j = strlen(c);
/* 6 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s%s%s%s%s%s%s", c,c,c,c,c,c, ss[g]);
                        printf("\t%s\n", ss[g++]);
                        if(g>=acnt) {strcpy(c, ""); continue;}
                        sscanf(s, "%s%s%s%s%s%s%s%s", c,c,c,c,c,c,c,c); j = strlen(c);
/* 7 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s%s%s%s%s%s%s%s", c,c,c,c,c,c,c, ss[g]);
                        printf("\t%s\n", ss[g++]);
                        if(g>=acnt) {strcpy(c, ""); continue;}
                        sscanf(s, "%s%s%s%s%s%s%s%s%s", c,c,c,c,c,c,c,c,c); j = strlen(c);
/* 8 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s%s%s%s%s%s%s%s%s", c,c,c,c,c,c,c,c, ss[g]);
                        printf("\t%s\n", ss[g++]);
                        if(g>=acnt) {strcpy(c, ""); continue;}
                        sscanf(s, "%s%s%s%s%s%s%s%s%s%s", c,c,c,c,c,c,c,c,c,c); j = strlen(c);
/* 9 */                 ss[g] = malloc(j*sizeof(char));
                        sscanf(s, "%s%s%s%s%s%s%s%s%s%s", c,c,c,c,c,c,c,c,c, ss[g]);
                        printf("\t%s\n", ss[g]);
					   }
					   free(c);
		}
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
				case 'h':   usage(0);
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
						usage(1);
						break;
					}
					midi_outdev = atoi(&(argv[1][2]));
					if(midi_outdev < 1){
						fprintf(stderr,"\nMIDI Out device must be > 0");
						usage(1);
						break;
					}
					break;
				case 'i':
					if(argv[1][2]== '\0'){
						fprintf(stderr,"\n-i flag needs a device number");
						usage(1);
						break;
					}
					midi_indev = atoi(&(argv[1][2]));
					if(midi_indev <1){
						fprintf(stderr,"\nMIDI In device must be > 0");
						usage(1);
						break;
					}
					break;
				case 'M':
                    listmidi = 1;
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
	    if(listmidi) usage(1);
	    else usage(0);
	}

	infiles = (FILES*)malloc(sizeof(FILES));

    /*int*/ len = strlen(argv[1]) + 5;
    ruleflnm = calloc(len, sizeof(char));
    strcpy(ruleflnm, argv[1]);

// Try opening, in order:
//    1. scriptname with user-included .tv or .tvd extension.
//    2. scriptname with added .tv extension - user omitted .tv on commandline.
//    3. scriptname with added .tvd extension - d signifies tv development script.
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

// Scan rule-file for use of midiin
	if(!infiles->encrypt) {	/* Can only test unencrypted scripts! */
    	char str[MAXSTRING]; char *ss;
    	int lineno = 1;
    	while(!feof(infile)) {
    	    if(fgets(str, MAXSTRING, infile)) {
    	        if(strncmp(str, "//", 2) == 0) continue;
    	        ss = str;
    	        for(; *ss != '\0'; ++ss) {
    	            if(memcmp("midiin", ss, 6) == 0) {
    	                midiin_used = 1;
    	                break;
    	            }
    	        }
    	        if(midiin_used) {
					break;
				}
    		}
    	}
		rewind(infile);
	}


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
		fprintf(stderr,
			"Ready to run performance (Press Enter key: CONTROL-C to end):\n");
		getchar();
	}
	else
		printf("Running... CONTROL-C to end:\n");
	perf_time(0);
	execute(prog);
	tidy_up(1);
	return 0;
}
