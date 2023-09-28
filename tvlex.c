#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "tv.h"
#include "xtab.h"
#define MAXSYM (100)

/* static union { */
/*       char    c[8]; */
/*       int32_t v[2]; */
/* } data; */

/* static int charp = 8; */

int	linenum = 1;
static Symbol *symtab = 0;

/* static void decode(long* v) */
/* { */
/*     unsigned long n=32, sum, y=v[0], z=v[1], */
/*              delta=0x9e3779b9 ; */
/*     sum = delta<<5 ; */
/*     /\* start cycle *\/ */
/*     while (n-->0) { */
/*       z   -= (y<<4)+k2 ^ y+sum ^ (y>>5)+k3 ; */
/*       y   -= (z<<4)+k0 ^ z+sum ^ (z>>5)+k1 ; */
/*       sum -= delta ; */
/*     } */
/*     /\* end cycle *\/ */
/*     v[0]=y ; v[1]=z ; */
/* } */

/* static int enc_getc(FILE* f) */
/* { */
/*     if (data.c[charp]=='\0') return EOF; */
/* /\*     putc(data.c[charp], stderr); *\/ */
/*     return data.c[charp++]; */
/* } */

static int tv_getc(FILES *infile)
{
      return getc(infile->fd);
}

static void tv_fgets(char *b, int len, FILES *infile)
{
    fgets(b, len, infile->fd);
}

static int tv_ungetc(int ch, FILES *infile)
{
    ungetc(ch, infile->fd);
    return 0;
}


Symbol *
lookup(char *str)
{
	Symbol *sp = symtab;
	while(sp != NULL) {
		if(str[0] == sp->picture[0] && strcmp(str, sp->picture) == 0)
			return sp;
		sp = sp->next;
	}
	if((sp = (Symbol *)calloc(1, sizeof(Symbol))) == 0) {
		yyerror("Can't get memory for symbol");
		exit(1);
	}
	sp->next = symtab;
	symtab = sp;
	if((sp->picture = calloc(1, strlen(str)+1)) == 0) {
		yyerror("Can't get memory for symbol picture");
		exit(1);
	}
	strcpy(sp->picture, str); /* Safe by construction */
	sp->keywcode = 0;
	return sp;
}

void
initsymtab(void)
{
	struct keytab *key = keytab;
	Symbol *sym;
	while(key->picture != 0) {
//        fprintf(stderr, "KEY->PICTURE is %s\n", key->picture);
		sym = lookup(key->picture);
		sym->keywcode = key->code;
		sym->rule = key->rule;
		sym->mfunc = key->mfunc;
		key++;
	}
}

static int
getstring(FILES *infile)
{
	char buf[MAXSTRING];
	char *bp = buf;
	int ch;
	for(;;) {
		if(bp - buf >= MAXSTRING-1) {
			fprintf(stderr, "TV: string constant too long on line %d\n",
							linenum);
			exit(1);
		}
		*bp = tv_getc(infile);
		switch(*bp) {
		case '"':
			*bp = '\0';
			yylval.string = emalloc(strlen(buf)+1);
			strcpy(yylval.string, buf); /* Safe by construction */
			return STRING;
		case '\n':
			fprintf(stderr, "TV: string constant contains newline on line %d\n",
							linenum);
			exit(1);
		case '\\':
			switch(ch = tv_getc(infile)) {
			case '0':
				*bp = '\0';
				break;
			case 'a':
				*bp = '\a';
				break;
			case 'b':
				*bp = '\b';
				break;
			case 'n':
				*bp = '\n';
				break;
			case '\\':
				*bp = '\\';
				break;
			case 'r':
				*bp = '\r';
				break;
			case 't':
				*bp = '\t';
				break;
				/* etc... */
			default:
				*bp = ch;
				break;
			}
			break;
		case EOF:
			fprintf(stderr, "TV: EOF in string constant\n");
			exit(1);
		}
		bp++;
	}
}

static void
getnumber(FILES *infile, int ch)
{
	char	buf[MAXSYM];
	char	*cp = buf;
	if(ch != '.') {
		*cp++ = ch;
		while(isdigit(ch = tv_getc(infile)))
		{
			if(cp - buf >= MAXSYM)
			{
				yyerror("badly formed number");
				exit(1);
			}
			*cp++ = ch;
		}
	}

	if(ch == '.')
	{
		*cp++ = '.';
		while(isdigit(ch = tv_getc(infile)))
		{
			if(cp - buf >= MAXSYM)
			{
				yyerror("badly formed number");
				exit(1);
			}
			*cp++ = ch;
		}
	}
	tv_ungetc(ch, infile);
	*cp++ = '\0';
	yylval.number = atof(buf);
}

int
yylex(void)
{
	int	ch, ch1;
	char	buf[MAXSYM];
	char	*cp;
	static int showcr = 0;
again:
	cp = buf;
	while(isspace(ch = tv_getc(infiles)))
		if(ch == '\n') {
			linenum++;
			if(showcr) {
				showcr = 0;
				return CR;
			}
		}
	if(ch == '/') {
		if((ch = tv_getc(infiles)) == '/') {	/* skip comments */
			while((ch = tv_getc(infiles)) != '\n')
				;
			linenum++;
			if(showcr) {
				showcr = 0;
				return CR;
			}
			goto again;
		} else {
			tv_ungetc(ch, infiles);
			ch = '/';
		}
	}
	showcr = 0;
	switch(ch) {
	case ',':
	case ':':
	case ';':
	case '{': case '}':
	case '(':
	case '^':
		return ch;
	case '[':
		if((ch = tv_getc(infiles)) == '|')
			return IMODO;
		tv_ungetc(ch, infiles);
		return '[';
	case '+': case '-':
		if((ch1 = tv_getc(infiles)) == ch) {
			showcr = 1;
			return (ch == '+') ? PLUSPLUS : MINUSMINUS;
		}
		tv_ungetc(ch1, infiles);
		/* FALLTHROUGH */
	case '*': case '/': case '%':
		if((ch1 = tv_getc(infiles)) == '=')
			switch(ch) {
			case '+':	return PLUSEQ;
			case '-':	return MINUSEQ;
			case '*':	return TIMESEQ;
			case '/':	return DIVEQ;
			case '%':	return MODEQ;
			}
		tv_ungetc(ch1, infiles);
		return ch;
	case ']':
	case ')':
		showcr = 1;
		return ch;
	case '<':
		if((ch = tv_getc(infiles)) == '=')
			return LESS_EQ;
		if (ch == '<') return BEGCAT;
		tv_ungetc(ch, infiles);
		return '<';
	case '>':
		if((ch = tv_getc(infiles)) == '=')
			return GTHAN_EQ;
		if (ch == '>') return ENDCAT;
		tv_ungetc(ch, infiles);
		return '>';
	case '&':
		if(tv_getc(infiles) != '&')
			yyerror("single '&' seen");
		return ANDAND;
	case '|':
		if((ch = tv_getc(infiles)) == '|')
			return OROR;
		else if(ch == ']') {
			showcr = 1;
			return IMODC;
		}
		tv_ungetc(ch, infiles);
		return '|';
	case '=':
		if((ch = tv_getc(infiles)) == '=')
			return EQ;
		tv_ungetc(ch, infiles);
		return '=';
	case '!':
		if((ch = tv_getc(infiles)) != '=')
			yyerror("badly formed '!='");
		return '!';
	case '"':
		return getstring(infiles);
		break;
	case '.':
		getnumber(infiles, '.');
		showcr = 1;
		return NUMBER;
		break;
	case EOF:
		{
		  FILES *x = infiles->next;
		  if (x == NULL) return EOF;
/* Closing included file */
		  fclose(infiles->fd);
		  free(infiles->name);
		  free(infiles);
		  infiles = x;
		  linenum = x->linenum;
		  goto again;
		}
	case '#':
		{
			char name[1024];
			FILES *x;
			tv_fgets(name,1024,infiles);
			if (strncmp(name, "include ", 8)==0) {
				char *p = name+8;
				while (*p==' ') p++;
				if (*p=='"') p++;
				else {
					printf("Syntax error in #include\n");
					goto again;
				}
				p = strtok(p, "\"\n");
				x = (FILES*)malloc(sizeof(FILES));
				infiles->linenum = linenum;
				x->next = infiles;
				if ((x->fd = fopen(p, "r"))==NULL) {
					printf("Failed to include file %s\n", p);
					free(x);
					goto again;
				}
				{	    /* Is this an encrypted file?? */
					int ch = getc(x->fd);
					ungetc(ch, x->fd);
/*		      fprintf(stderr, "Enc (%s) = %d\n", p, infiles->encrypt); */
				}
				x->name = (char*)malloc(strlen(p)+1);
				strcpy(x->name, p); /* Safe by construction */
				linenum = 1;
				infiles = x;
				goto again;
			}
			printf("Failed to include file\n");
			goto again;
		}
	}
	if(isdigit(ch)) {
		getnumber(infiles, ch);
		showcr = 1;
		return NUMBER;
	}
	if(isalpha(ch) || ch == '_' || ch == '\'') {
		*cp++ = ch;
		while(isalpha(ch = tv_getc(infiles)) ||
		      isdigit(ch) || ch == '_') {
		  if(cp - buf >= MAXSYM) {
			        yyerror("badly formed symbol");
				exit(1);
			}
			*cp++ = ch;
		}
		*cp++ = '\0';
		tv_ungetc(ch, infiles);
		yylval.symbol = lookup(buf);
		if(yylval.symbol->keywcode != 0) {
			if(yylval.symbol->keywcode == BREAK
			   ||yylval.symbol->keywcode == CONTINUE
			   ||yylval.symbol->keywcode == RETURN
			   ||yylval.symbol->keywcode == LOOP)
				showcr = 1;
			return yylval.symbol->keywcode;
		}
		if(yylval.symbol->rule != 0) {
			yylval.rule = yylval.symbol->rule;
			showcr = 1;
			return RULE;
		}
		if(yylval.symbol->mfunc != 0) {
			yylval.mfunc = yylval.symbol->mfunc;
			return MFUNC;
		}
		showcr = 1;
		return istablesym(yylval.symbol) ? TIDENT :
			isstablesym(yylval.symbol) ? SIDENT : IDENT;
	}
	sprintf(buf, "Syntax error - got char '%c' - ignored", ch);
	yyerror(buf);
	goto again;
}

int
istablesym(Symbol *sym)
{
	return isupper(sym->picture[0]);
}

int
isstablesym(Symbol *sym)
{
    return ('\''==sym->picture[0]);
}
