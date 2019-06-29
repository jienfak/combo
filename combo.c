#include "arg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define WORDS_MAX 64

char *argv0;
static char *opt_nwords  = NULL;
static char *opt_maxreps = NULL;
static char *opt_seps    = NULL;
static char *opt_io      = NULL;
static int  reverse = 0;
static char **words;
static int  words_amount;

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
	die("usage: %s [-vr] [-m max_reps] [-n words_number] [-o file] [-s seps] [words]\n" , argv0);
}

uint64_t chomp(char *str)
{
	uint64_t i = strlen(str);
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

int
main(int argc, char **argv)
{
	uint64_t buflen;
	char buf[BUFSIZ];
	ARGBEGIN {
	case 'm':
		opt_maxreps = EARGF(usage());
		break;
	case 'n':
		opt_nwords = EARGF(usage());
		break;
	case 'o':
		opt_io = EARGF(usage());
	case 'v':
		die("%s " VERSION "\n", argv0);
		break;
	case 's':
		opt_seps = EARGF(usage());
		break;
	case 'r':
		reverse = 1;
	default:
		usage();
	} ARGEND;
	
	words_amount = argc;
	words = malloc(sizeof(char *) * WORDS_MAX);
	while (argc && argv) {
		/* Get words from arguments. */
		words[words_amount-(argc--)] = *argv++ ;
	}
	if (! words_amount) {
		/* Get words from stdin. */
		while ( fgets(buf, sizeof(buf), stdin)) {
			/* Memory allocation and copying from buffer. */
			buflen = chomp(buf);
			words[words_amount] = malloc(sizeof(char) * buflen);
			strcpy(words[words_amount], buf);

			++words_amount;
		}
	}
	for (int i=0 ; i<words_amount ; ++i) {
		printf("%s\n", words[i]);
	}

	return 0;
}
