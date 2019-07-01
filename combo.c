#include "arg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef unsigned long ul;
typedef unsigned short us;


#define WORDS_MAX 256

char *argv0;
static char *opt_nwords  = NULL;
static char *opt_maxreps = NULL;
static char *opt_seps    = NULL;
static char *opt_out     = NULL;
static char *opt_in      = NULL;
static us opt_stdin = 0;
static us opt_rev   = 0;

static char **words;
static ul  wa;
static ul wa0;
/* Standard input and outputs. */
static FILE *input;
static FILE *output;

void   die(const char *errstr, ...);
void   usage(void);
ul     chomp(char *str);
char   *strrev(char *dest, char *str);
ul     power(ul num, us n);

void openinput(void);
void openstdin(void);
void openoutput(void);
void openstdout(void);
void printcombos(void);
void getwordsfrominput(void);
void addwordsfrominput(void);

void
die(const char *errstr, ...)
{
	va_list ap;
	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(1);
}

void
usage(void)
{
	die("usage: %s [-vr] [-i in_file] [-m max_reps] [-n words_number] [-o out_file] [-s seps] [words]\n" , argv0);
}

ul
chomp(char *str)
{
	ul i = strlen(str);
	if (str[i-1] == '\n'){
		if (str[i-2] == '\r') {
			str[i-2] = '\0';
			return i-2;
		}
		str[i-1] = '\0';
		return i-1;
	}
	return i;
}

char
*strrev(char *dest, char *str){
	char *pbuf = malloc( sizeof(char) * strlen(str) );
	char *pbuf0 = pbuf;
	ul len = 0;

	while(*str) ++str, ++len;

	/* To escape zero end character. */
	--str;
	for(; len ; --len){
		*pbuf++ = *str--;
	}

	*pbuf = '\0';
	strcpy(dest, pbuf0);

	free(pbuf0);

	return dest ;
}

ul
power(ul num, us n)
{
	ul buf = 1;
	while (n--) buf *= num;
	return buf;
}

void
printcombos(void)
{
	return;
	ul c = 0;
	for(;;){}
}

void
openinput(void)
{
	if (opt_in) {
		if(!(input=fopen(opt_in, "r"))){
			perror("fopen");
			exit(1);
		}
	} else {
		openstdin();
	}
}

void
openstdin(void)
{
	input = stdin;
}


void
openoutput(void)
{
	if (!(output=fopen(opt_in, "w"))) {
		perror("fopen");
		exit(1);
	}
}

void
openstdout(void)
{
	output = stdout;
}

void
getwordsfrominput(void)
{
	ul buflen;
	char buf[BUFSIZ];
	/* Get words from file. */
	while ( fgets(buf, sizeof(buf), input)) {
		/* Memory allocation and copying from buffer. */
		buflen = chomp(buf);
		words[wa] = malloc(sizeof(char) * buflen);
		strcpy(words[wa], buf);
		++wa;
	}
}

void
addwordsrev(void)
{
	ul i1, i2;
	ul wa2 = wa+wa0;
	for (i1=0, i2=wa; i1<wa2; ++i1, ++i2){
		/*
		 * They are placed with offset=wa0
		 * [1][2][3][4][5][6]
		 *     bias
		 *  ^---------^
		 *     ^--------^
		 *        ^--------^
		 */
		words[i2] = malloc( sizeof(char)*strlen(words[i1]) );
		strrev(words[i2], words[i1]);
	}
	wa = wa2;
}


int
main(int argc, char **argv)
{
	ARGBEGIN {
	case 'i':
		opt_in = EARGF(usage());
		break;
	case 'm':
		opt_maxreps = EARGF(usage());
		break;
	case 'n':
		opt_nwords = EARGF(usage());
		break;
	case 'o':
		opt_out = EARGF(usage());
	case 'v':
		die("%s " VERSION "\n", argv0);
		break;
	case 's':
		opt_seps = EARGF(usage());
		break;
	case 'r':
		opt_rev = 1;
		break;
	case '+':
		opt_stdin = 1;
		break;
	default:
		usage();
	} ARGEND;
	
	wa = argc;
	words = malloc(sizeof(char *) * WORDS_MAX);
	while (argc && argv) {
		/* Get words from arguments. */
		words[wa-(argc--)] = *argv++ ;
	}

	if ( !wa || opt_in ) {
		/* Get words from input file. */
		openinput();
		getwordsfrominput();
		fclose(input);
	}
	wa0 = wa;

	if (opt_stdin) {
		/* Read from stdin. */
		openstdin();
		getwordsfrominput();
	}

	if (opt_rev)
		/* Additional words. */
		addwordsrev();

	if (opt_out)
		/* Output file. */
		openoutput();


	for (int i=0 ; i<wa ; ++i) {
		printf("%s\n", words[i]);
	}
	printcombos();

	return 0;
}
