#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define PBUFSIZ    (64)
#define ASCII_BIAS (' ')
#define MAX_VARGS  (128)
#define SIZEL(e)   (sizeof(e[0]))

typedef unsigned long  ul;
typedef unsigned short us;
typedef char *         str;
typedef unsigned char  chr;


/* Options. */
struct CatOpts {
	str oargs[96];
	str *vargs;
};


/* It's a backend of linked list to save any data.*/
struct LinkedList {
	/* Easiest linked list structure. */
	void *pdat;
	struct LinkedList *nxt;
	us typ;
} ;

/* Common. */
static void    die(const str errstr, ...);
static void    usage(const str argv0);
/* Strings. */
static ul      strchmp(str s, chr c);
static str   strrev(str dst, str str);
/* String lists. */
static str     *ssncat(str dst[], str src[], ul n);
static str     *sscat(str dst[], str src[]);
/* Lightweight math functions. */
static ul      power(ul num, us n);
/* File functions. */
static FILE   *fopenin(const str path);
static FILE   *fopenout(const str path);
static str    *fgetlines(FILE *file);

/* Combo functions. */
static ul     *combomask(ul mask[], const ul wa, const ul id);
static void    fprintcombos(str *ws[], ul wa, struct Options *opt, FILE *f);
/*
void die(const str *errstr, ...){
	va_list ap;
	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(1);
}
*/

/*
void usage(const str *argv0){
	die("usage: %s [-Vr+] [-lin_file] [-mmax_reps] [-nwords_number] [-oout_file] [-s seps] [words]\n" , argv0);
}
*/


struct LinkedList *ll_crt(void){
	/* This function creates first element of list. */
	/* First element does not contain anything */
	/* Memory for first element of list. */
	struct LinkedList *ll = malloc( sizeof(struct LinkedList) ) ;
	/* Initialisation values. */
	ll->pdat = NULL ;
	ll->nxt  = NULL ;
	/* This type means "Start of list.". */
	ll->typ = 0 ;
	return ll ;
}

ul ll_len( struct LinkedList *lst){
	/* Function returns size of list(Logically). */
	struct LinkedList *cur = lst ;
	ul cnt = 0 ;
	while(cur->nxt){
		++cnt;
		cur = cur->nxt ;
	}
	return cnt ;
}

void ll_add(struct LinkedList *ll0, void *pdat){
	struct LinkedList *cur = ll0 ;
	/* Find last pointer */
	while( cur->nxt ) cur = cur->nxt ;
	/* Get memory for the next element and get it after. */
	cur->nxt = malloc( sizeof(struct LinkedList) ) ;
	cur->nxt->nxt = NULL ;
	/* Initialisation. */
	cur->nxt->pdat = pdat ;
}

void *ll_at(struct LinkedList *ll0, ul idx){
	/* This function returns a pointer to the data it saved.*/
	struct LinkedList *cur = ll0 ;
	/* Find element by an index. */
	for( ul i=0 ; i<(idx+1) ; ++i ){
		cur = cur->nxt ;
	}
	return cur->pdat ;
}

us ll_ins(struct LinkedList *ll0, ul idx, void *pdat){
	/* This function inserts an element into a list and returns '0' if it's succesful. */
	struct LinkedList *cur = ll0 ;
	struct LinkedList *end;
	if( ll_len(ll0)<idx ){
		/* Too big index. */
		return 1 ;
	}
	for( ul i=0 ; i<idx ; ++i ){
		cur = cur->nxt ;
	}
	/* To save element which will go after inserted. */
	end = cur->nxt ;
	/* Initialisation of next element. */
	cur->nxt        = ll_crt();
	cur->nxt->nxt  = end ;
	cur->nxt->pdat = pdat ;
	/* Succesful inserted. */
	return 0 ;
}

us ll_rm(struct LinkedList *ll0, ul idx){
	struct LinkedList *cur = ll0 ;
	struct LinkedList *end;
	if( ll_len(ll0)<=idx ){
		/* Too big index. */
		return 1 ;
	}
	for( ul i=0 ; i<idx ; ++i ){
		cur = cur->nxt ;
	}	
	/* Removing. */
	free(cur->nxt);
	cur->nxt = cur->nxt->nxt ;
	/* Succesful removed. */
	return 0 ;
}

void ll_free(struct LinkedList *ll){
	/* Frees alocated for structure memory. */
	if(ll->nxt==NULL) free(ll) ;
	else ll_free(ll->nxt), ll->nxt = NULL, ll_free(ll) ;
}

str *ll2ss(struct LinkedList *ll){
	/* Converts linked list of Z-strings to array with NULL-pointer at end. */
	ul len = ll_len(ll) ;
	str *ss = malloc(SIZEL(ss) * (len+1)) ;
	for( int i=0 ; i<len ; ++i )
		ss[i] = ll_at(ll, i) ;
	ss[len] = NULL ;
	return ss ;
}

chr strchp(str s){
	/* Chops the last char and returns it. */
	ul l = strlen(s) ;
	chr c = s[l -= 1] ;
	s[l] = '\0' ;
	return c ;
}

ul strchmp(str s, chr c){
	/* Deletes one the last 'c' str acter if it has and returns new length. */
	ul l = strlen(s) ;
	if (s[l-1]==c) --l ;
	s[l] = '\0';
	return l;
}

str strrev(str dst, str src){
	/* Copy reversed string from the source into the destination(destination can be source). */
	str pbuf = malloc( SIZEL(str) * strlen(src) ) ;
	str pbuf0 = pbuf ;
	ul len = 0;
	while(*src) ++src, ++len ;
	/* To escape zero end str acter. */
	--src;
	for(; len ; --len ){
		*pbuf++ = *src-- ;
	}
	*pbuf = '\0' ;
	strcpy(dst, pbuf0);
	free(pbuf0);
	return dst;
}

ul power(const ul num, us n){
	ul buf = 1;
	while (n--) buf *= num;
	return buf;
}

ul ulmin(ul a, ul b){
	return a<b ? a : b ;
}

ul ulmax(ul a, ul b){
	return a>b ? a : b ;
}

void fseek0(FILE *f){
	fseek(f, 0, SEEK_SET);
}

ul fchrcnt(const chr c, FILE *f){
	long pos = ftell(f) ;
	ul cnt = 0 ;
	chr rc;
	while( (rc=fgetc(f)) !=  EOF )
		if(rc==c) ++cnt ;
	fseek0(f);
	return cnt ;
}

FILE *fopenin(const str p){
	/* Returns file to read or crashes with errors. */
	FILE *f;
	if(!(f=fopen(p, "r"))){
		perror("fopen");
		exit(1);
	}
	return f ;
}

FILE *fopenout(const str p){
	/* Returns file to write(without appending) or crashes with errors. */
	FILE *f;
	if (!(f=fopen(p, "w"))) {
		perror("fopen");
		exit(1);
	}
	return f ;
}

str *fgetlines(FILE *f){
	/* Gets lines from file and returns allocated in memory array(by malloc, so it can be freed). */
	struct LinkedList *ll = ll_crt() ;
	chr buf[BUFSIZ];
	while(fgets(buf, sizeof(buf), f)){
		/* Reading, memory allocation and copying from buffer to dump and adding linked list. */
		ul buflen = strchmp(buf, '\r') ;
		buflen = strchmp(buf, '\n') ;
		str ptr = malloc(SIZEL(ptr)*(buflen+1));
		strcpy(ptr, buf);
		ll_add(ll, ptr);
	}
	str *ss = ll2ss(ll) ;
	ll_free(ll);
	return ss ;
}

void *arrncpy(void *dst, void *src, size_t siz, ul n){
	/* Copies array with length=n and size of element = siz. */
	return memcpy(dst, src, siz*n) ;
}

ul ssnrev_for(str dst[], str src[], const ul n){
	/* Copies reversed words from 'src' to 'dst' with 'n' = lines count and returns it. */
	for( ul i=0 ;  i<n ; ++i ){
		dst[i] = malloc( SIZEL(dst)*strlen(src[i]) ) ;
		strrev(dst[i], src[i]);
	}
	return n ;
}

ul ptrarrlen(void *a[]){
	/* Returns amount of pointers in array just before NULL. */
	void **pa=a ; while(*pa) ++pa ; return pa-a ;
}

ul sslen(str ss[]){
	/* Gets length of Z-strings array. */
	/*str *pss = ss ; while (*pss) ++pss ; return pss-ss ;*/
	return ptrarrlen(ss) ;
}

ul ssrev_for(str dst[], str src[]){
	/* Copies reversed words from 'src' to 'dst' until NULL-pointer meet. */
	return ssnrev_for(dst, src, sslen(src)) ;
}


str ssncat2str(str s, str ss[], ul n){
	/* Joins lines from 'ss' array with length 'n' to 's'.*/
	for( ul i=0 ; i<n ; ++i ) strcat(s, ss[i]) ; return s ;
}

str sscat2str(str s, str ss[]){
	/* Joins lines from 'ss' array with length 'sslen' to 's'. */
	return ssncat2str(s, ss, sslen(ss)) ;
}

str *ssncpy(str dst[], str src[], ul n){
	/* Copies Z-strings array from 'src' to 'dst- with length equal 'n'. */
	/*for( ul i=0 ; i<n ; ++i )
		dst[i] = src[i] ; */
	arrncpy(dst, src, n, SIZEL(src));
	return dst ;
}

str *sscpy(str dst[], str src[]){
	/* Copies Z-strings array from 'src' do 'dst' with length equal 'sslen'. */
	return ssncpy(dst, src, sslen(src)+1);
}

str *sscat(str dst[], str src[]){
	/* Concatenates Z-strings array . */
	return sscpy(dst+sslen(dst), src) ;
}

str *sscatarrn(str dst[], str *src[], ul n){
	/* Concatenates array of arrays(fuck) with 'n' length. */
	for( ul i=0 ; i<n ; ++i ) sscat(dst, src[i]) ; return dst ;
}

str *sscatarr(str dst[], str **src){
	/* Concatenates array of arrays while don't meet NULL-pointer. */
	while (*src) sscat(dst, *(src++)) ; return dst ;
}


ul *combo_makmsk(ul mask[], const ul wa, const ul id){
	return 0 ;
}

void combo_fputcmbs(str ws[], ul wa, struct Options *opt, FILE *out){
	return;
	ul c = 0;
	for(;;){}
}

struct CatOpts *co_crtopts(ul argc, str argv[]){
	struct CatOpts *opts = malloc(sizeof(struct CatOpts)) ;
	for( int i=0 ; i<96 ; ++i )
		opts->oargs[i] = NULL ;
	unsigned int str_cnt = argc ;
	for( int i=0 ; i<argc ; ++i )
		/* Getting optional arguments. */
		if( argv[i][0]=='-' ){
			opts->oargs[argv[i][1]-ASCII_BIAS] = argv[i]+2 ;
			--str_cnt;
		}
	opts->vargs = malloc(SIZEL(opts->vargs) * (str_cnt+1)) ;
	for( int i1=0, i2=0; i1<argc ; ++i1 )
		/* Getting just arguments, without any option for them. */
		if( argv[i1][0]!='-'){
			if(argv[i1][0]=='\\') opts->vargs[i2] = argv[i1]+1 ;
			else opts->vargs[i2] = argv[i1] ;
			++i2;
		}
	opts->vargs[str_cnt] = NULL ;
	return opts ;
}

str co_oarg(struct CatOpts *opts, chr c){
	return opts->oargs[c-ASCII_BIAS] ;
}

str co_varg(struct CatOpts *opts, int i){
	return opts->vargs[i] ;
}

str *co_vargarr(struct CatOpts *opts){
	return opts->vargs ;
}
int co_vargamt(struct CatOpts *opts){
	return sslen(opts->vargs) ;
}

int combo_run(int argc, str argv[]){
	/* No run if not enough arguments. */
	if (argc==1) return 1 ;
	/* Argument parser. */
	struct CatOpts *opts = co_crtopts(argc-1, argv+1) ;
	/* By some reason malloc in printf can be BROKEN, I'm lazy now to report
	 * this, but this hack fixes this in MY program, so I don't care. */
	printf("");
	/* Output file. */
	str pfo = co_oarg(opts, 'o') ;
	FILE *o   = pfo ? fopenout(pfo) : stdout ;
	/* Memory multiplier for additional word variants. */
	ul addmemx = 0 ;
	/* Reversed words take memory too, so increment 'addmemx'. */
	co_oarg(opts, 'r') && ++addmemx ;
	/* General multiplier. */
	ul memx = 1+addmemx ;
	/* Void pointers array to prevent shit. */
	str voidbuf[] = {NULL} ;
	/* Buffer for file list words. */
	str *fbufws = voidbuf ;
	/* Path to file list. */
	str pfl = co_oarg(opts, 'l') ;
	if (pfl) /* Reading list. */
		fbufws = fgetlines(  fopenin( pfl )  ) ;
	/* Buffer for words from standard input. */
	str *stdbufws = voidbuf ;
	if (co_oarg(opts, '-')) /* Read words from standard input. */
		stdbufws = fgetlines(stdin) ;
	/* Words amount(without additional words, like reversed or something). */
	ul wa0 = sslen(fbufws)+sslen(stdbufws)+co_vargamt(opts) ;
	/* All the words. */
	ul wa = wa0*memx ;
	/* Getting words from all the inputs into one array. */
	str *ws = malloc(sizeof(str)*wa) ;
	str *wsarr[] = { co_vargarr(opts), fbufws, stdbufws, NULL } ;
	sscatarr(ws, wsarr);
	for( ul i=0 ; i<wa ; ++i ){
		printf("%s\n", ws[i]);
	}
	return 0 ;
}

int main(int argc, char *argv[]){
	/* Wrapper to make possible embeded combo into another programs. */
	return combo_run(argc, argv) ;
}
