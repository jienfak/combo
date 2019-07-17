#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define PBUFSIZ 64

#define USAGE (usage(argv0))
/* suckless 'arg.h' */
#define ARGBEGIN	for (argv0 = *argv, argv++, argc--;\
					argv[0] && argv[0][0] == '-'\
					&& argv[0][1];\
					--argc, ++argv) {\
				char argc_;\
				char **argv_;\
				int brk_;\
				if (argv[0][1] == '-' && argv[0][2] == '\0') {\
					++argv;\
					--argc;\
					break;\
				}\
				for (brk_ = 0, argv[0]++, argv_ = argv;\
						argv[0][0] && !brk_;\
						argv[0]++) {\
					if (argv_ != argv)\
						break;\
					argc_ = argv[0][0];\
					switch (argc_)
#define ARGEND			}\
			}

#define ARGC()		argc_

#define EARGF(x)	((argv[0][1] == '\0' && argv[1] == NULL)?\
				((x), abort(), (char *)0) :\
				(brk_ = 1, (argv[0][1] != '\0')?\
					(&argv[0][1]) :\
					(argc--, argv++, argv[0])))

#define ARGF()		((argv[0][1] == '\0' && argv[1] == NULL)?\
				(char *)0 :\
				(brk_ = 1, (argv[0][1] != '\0')?\
					(&argv[0][1]) :\
					(argc--, argv++, argv[0])))

typedef unsigned long ul;
typedef unsigned short us;

/* Options. */
struct Opts {
	char  *oargs[256];
	char **vargs;
};
/* Common. */
static void    die(const char *errstr, ...);
static void    usage(const char *argv0);
/* Strings. */
static ul      strchop(char *s, char c);
static char   *strrev(char *dst, char *str);
/* String lists. */
static ul      strsrev(char *dst[], char *src[], const ul la);
static ul      strslen(char **strs);
static char   *strsncat(char *dst, char *strs[], ul n);
static char   *strscat(char *dst, char *strs[]);
/* Lightweight math functions. */
static ul      power(const ul num, us n);
/* File functions. */
static FILE   *fopenin(const char *path);
static FILE   *fopenout(const char *path);
static ul      fgetlines(char *lines[], FILE *file);

/* Combo functions. */
static ul     *combomask(ul mask[], const ul wa, const ul id);
static void    fprintcombos(char *ws[], ul wa, struct Options *opt, FILE *f);

void die(const char *errstr, ...){
	va_list ap;
	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(1);
}

void usage(const char *argv0){
	die("usage: %s [-Vr+] [-a maxwordsamount] [-i in_file] [-m max_reps] [-n words_number] [-o out_file] [-s seps] [words]\n" , argv0);
}

ul strchp(char *s, char c){
	/* It deletes one the last 'c' character if it has and returns new length. */
	ul l = strlen(s);
	if (s[l-1]==c) --l;
	s[l] = '\0';
	return l;
}

char *strrev(char *dst, char *src){
	/* Copy reversed string from the source into the destination. */
	char *pbuf = malloc( sizeof(char) * strlen(src) );
	char *pbuf0 = pbuf;
	ul len = 0;

	while(*src) ++src, ++len;

	/* To escape zero end character. */
	--src;
	for(; len ; --len){
		*pbuf++ = *src--;
	}
	*pbuf = '\0';

	strcpy(dst, pbuf0);
	free(pbuf0);
	return dst;
}

ul power(const ul num, us n){
	ul buf = 1;
	while (n--) buf *= num;
	return buf;
}

FILE *fopenin(const char *path){
	/* Returns file to read or crashes with errors. */
	FILE *file;
	if(!(file=fopen(path, "r"))){
		perror("fopen");
		exit(1);
	}
	return file;
}

FILE *fopenout(const char *path){
	/* Returns file to write(without appending) or crashes with errors. */
	FILE *file;
	if (!(file=fopen(path, "w"))) {
		perror("fopen");
		exit(1);
	}
	return file;
}

ul fgetlines(char *lines[], FILE *f){
	/* Gets lines from file and return amount of them. */
	ul la=0;
	ul buflen;
	char buf[BUFSIZ];
	while ( fgets(buf, sizeof(buf), f)) {
		/* Reading, memory allocation and copying from buffer. */
		buflen = strchop(buf, '\r');
		buflen = strchop(buf, '\n');
		lines[la] = malloc(sizeof(char) * buflen);
		strcpy(lines[la], buf);
		++la;
	}
	return la;
}

ul strsrev(char *dst[], char *src[], const ul la){
	/* Copies reversed words from 'src' to 'dst' with 'la' = lines count and returns it. */
	ul i;
	for (i=0;  i<la; ++i) {
		dst[i] = malloc( sizeof(char)*strlen(src[i]) );
		strrev(dst[i], src[i]);
	}
	return la;
}

ul strslen(char **strs){
	/* Get len of Z-strings array massive. */
	char **pstrs = strs;
	while (*pstrs++!=NULL);
	return pstrs-strs-1;
}

char *strsncat(char *dst, char *strs[], ul n){
	/* Joins lines from 'strs' array with size 'n' to 'dst'.*/
	for (ul i=0; i<n ; ++i) strcat(dst, strs[i]);
	return dst;
}

char *strscat(char *dst, char *strs[]){
	/* Joins lines from 'strs' array ending with NULL-pointer. */
	return strsnjoin(dst, strs, strslen(strs));
}

ul *combomask(ul mask[], const ul wa, const ul id){
}

void fprintcombos(char *ws[], ul wa, struct Options *opt, FILE *out){
	return;
	ul c = 0;
	for(;;){}
}

struct Opts *crtopts(int argc, char *argv[]){
	struct Opts opts = malloc(sizeof(struct Opts)) ;
	int str_cnt = argc ;
	for( int i=0 ; i<argc ; ++i ){
		if( argv[i][0]=='-' ){
			opts.oargs[argv[[i][1]] = argv[i]+2 ;
			--str_cnt;
		}
	}
	opts.vargs = malloc(sizeof(char *) * str_cnt) ;
	for( int i1=0, i2=0; i1<argc ; ++i1 )
		if( argv[i1][0]!='-'){
			if(argv[i1][0]=='\\') opts.vargs[i2] = argv[i1]+1 ;
			else opts.vargs[i2] = argv[i1] ;
			++i2;
		}
		
	return opts ;
}

int main(int argc, char *argv[]){
	struct Options opt = {
		.maxwordsamount = NULL,
		.in             = NULL,
		.maxreps        = NULL,
		.nwords         = NULL,
		.out            = NULL,
		.seps           = NULL,
		.rev            = 0,
		.std_in         = 0
	};
	struct Opts *opt = crtopts(argc-1, argv+1) ;
	char *argv0;
	ARGBEGIN {
		case 'a':
			opt.maxwordsamount = EARGF(USAGE);
			break;
		case 'i':
			opt.in = EARGF(USAGE);
			break;
		case 'm':
			opt.maxreps = EARGF(USAGE);
			break;
		case 'n':
			opt.nwords = EARGF(USAGE);
			break;
		case 'o':
			opt.out = EARGF(USAGE);
		case 'V':
			die("%s " VERSION "\n", argv0);
			break;
		case 's':
			opt.seps = EARGF(USAGE);
			break;
		case 'r':
			opt.rev = 1;
			break;
		case '+':
			opt.std_in = 1;
			break;
		default:
			USAGE;
	} ARGEND;
	ul wa = argc;
	char **ws = malloc( sizeof(char *) *
			(opt.maxwordsamount ? atoi(opt.maxwordsamount) : 1024) ) ;
	while (argc && argv) {
		/* Get words from arguments. */
		ws[wa-(argc--)] = *argv++ ;
	}

	if (opt.in) {
		/* If input option is set. */
		wa += fgetlines(&ws[wa], fopenin(opt.in)) ;
		if (opt.std_in)
			wa += fgetlines(&ws[wa], stdin) ;
	} else if (!wa || opt.std_in) {
		/* We have no words or have standard input option then read from it them. */
		wa += fgetlines(&ws[wa], stdin) ;
	}
	/* All the words we got from all the inputs counter. */
	ul wa0 = wa ;

	if (opt.rev)
		/* Additional reversed words.
		 * They are placed with offset='wa0'.
		 * [1][2][3][4][5][6]
		 *     bias
		 *  ^--------^
		 *     ^--------^
		 *        ^--------^                  */
		wa += strsrev(&ws[wa0], &ws[0], wa0) ;

	FILE *output;
	if (opt.out)
		/* Output file. */
		output = fopenout(opt.out) ;
	else
		/* Standard output. */
		output = stdout ;


	for (ul i=0 ; i<wa ; ++i) {
		printf("%s\n", ws[i]);
	}
	fprintcombos(ws, wa, &opt, output);

	return 0 ;
}
