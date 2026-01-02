#ifndef __ULISP_H
#define __ULISP_H

#include <inttypes.h>
#include <stdbool.h>

//#include <dirent.h>
#include <stdio.h>
#include <string.h>


#define BOARD_HAS_ExternalRAM

#ifdef BOARD_HAS_ExternalRAM
#define Bank1_SRAM1_ADDR    ((u32)(0x60000000))
#define DEF_ExternalRAM_SIZE	(1048576)//(524288)
#define DEF_ExternalRAM_WORKSPACE_SIZE	(DEF_ExternalRAM_SIZE/2)
#endif


/********** ARRAY2-system **********/
//#define DEF_ARRAY2
//#define graphics_package

#define filesys_commands

// Compile options

// #define resetautorun
#define printfreespace
// #define printgcs
#define sdcardsupport
#define gfxsupport
#define graphics_package

#define touchscreen_support

//#define tcp_keyboard
#define gfx_stdout
//#define wifi_support
#define lisplibrary

//#define assemblerlist
//#define lineeditor
#define vt100
#define extensions

//#define BOARD_HAS_PSRAM



#ifdef BOARD_HAS_PSRAM
#define WORKSPACESIZE (4096*16)                  /* Objects (8*bytes) */
#else
	#ifdef BOARD_HAS_ExternalRAM
	#define WORKSPACESIZE  (u32)(DEF_ExternalRAM_WORKSPACE_SIZE/8)
	#else
	#define WORKSPACESIZE (2000)
	#endif
#endif

#define MAX_STACK 7000

#define printfreespace


#include <setjmp.h>


#define TFT_BACKLITE 4

#ifdef  gfxsupport
#define  COLOR_WHITE 0xffff
#define  COLOR_BLACK 0
//#define tft = 5 ;
#endif


#define WORDALIGNED __attribute__((aligned (4)))
#define BUFFERSIZE 512  // Number of bits+4


#define nil                NULL
#define car(x)             (((object *) (x))->car)
#define cdr(x)             (((object *) (x))->cdr)

#define first(x)           car(x)
#define rest(x)            cdr(x)
#define second(x)          first(rest(x))
#define cddr(x)            cdr(cdr(x))
#define third(x)           first(cddr(x))

#define push(x, y)         ((y) = cons((x),(y)))
#define pop(y)             ((y) = cdr(y))

#define protect(y)         push((y), GCStack)
#define unprotect()        pop(GCStack)

#define integerp(x)        ((x) != NULL && (x)->type == NUMBER)
#define floatp(x)          ((x) != NULL && (x)->type == FLOAT)
#define symbolp(x)         ((x) != NULL && (x)->type == SYMBOL)
#define stringp(x)         ((x) != NULL && (x)->type == STRING)
#define characterp(x)      ((x) != NULL && (x)->type == CHARACTER)
#define arrayp(x)          ((x) != NULL && (x)->type == ARRAY)
#define streamp(x)         ((x) != NULL && (x)->type == STREAM)

#define mark(x)            (car(x) = (object *)(((uintptr_t)(car(x))) | MARKBIT))
#define unmark(x)          (car(x) = (object *)(((uintptr_t)(car(x))) & ~MARKBIT))
#define marked(x)          ((((uintptr_t)(car(x))) & MARKBIT) != 0)
#define MARKBIT            1

#define setflag(x)         (Flags = Flags | 1<<(x))
#define clrflag(x)         (Flags = Flags & ~(1<<(x)))
#define tstflag(x)         (Flags & 1<<(x))

#define issp(x)            (x == ' ' || x == '\n' || x == '\r' || x == '\t')
#define isbr(x)            (x == ')' || x == '(' || x == '"' || x == '#' || x == '\'')
#define fntype(x)          (getminmax((uint16_t)(x))>>6)
#define longsymbolp(x)     (((x)->name & 0x03) == 0)
#define longnamep(x)       (((x) & 0x03) == 0)
#define twist(x)           ((uint32_t)((x)<<2) | (((x) & 0xC0000000)>>30))
#define untwist(x)         (((x)>>2 & 0x3FFFFFFF) | ((x) & 0x03)<<30)
#define arraysize(x)       (sizeof(x) / sizeof(x[0]))

#define stringifyX(x)      #x
#define stringify(x)       stringifyX(x)
#define PACKEDS            0x43238000
#define BUILTINS           0xF4240000
#define ENDFUNCTIONS       0x0BDC0000


#define TRACEMAX 3  // Maximum number of traced functions
enum type { ZZERO=0, SYMBOL=2, CODE=4, NUMBER=6, STREAM=8, CHARACTER=10, FLOAT=12, ARRAY=14, ARRAY2=16, STRING=18, PAIR=20 };  // ARRAY STRING and PAIR must be last
enum token { UNUSED, BRA, KET, QUO, DOT };
enum stream { SERIALSTREAM, I2CSTREAM, SPISTREAM, SDSTREAM, WIFISTREAM, STRINGSTREAM, GFXSTREAM };
enum fntypes_t { OTHER_FORMS, TAIL_FORMS, FUNCTIONS, SPECIAL_FORMS };





#ifdef LINUX_X64

typedef uint64_t symbol_t;
typedef uint32_t chars_t;
typedef uint32_t builtin_t;

typedef int64_t integer_t;
typedef double sfloat_t;

typedef struct sobject {
  union
  {
    struct {
      sobject *car;
      sobject *cdr;
    };
    struct {
      //unsigned long int type;
      uintptr_t  type ;
      union {
        void*  pointer ;
        symbol_t name;
        long int integer;
        long int chars; // For strings
        sfloat_t single_float;
      };
    };
  };
} object;
#else
typedef uint32_t symbol_t;
typedef uint32_t builtin_t;
typedef uint32_t chars_t;
typedef float sfloat_t;
typedef int32_t integer_t;


typedef struct sobject {
  union {
    struct {
      sobject *car;
      sobject *cdr;
    } ;
    struct {
      unsigned int type;
      union {
    	uintptr_t pointer ;
        symbol_t name;
        int32_t integer;
        chars_t chars; // For strings
        sfloat_t single_float;
      };
    };
  };
} object;






#endif


typedef object *(*fn_ptr_type)(object *, object *);
typedef void (*mapfun_t)(object *, object **);

typedef struct {
  const char *string;
  fn_ptr_type fptr;
  uint8_t minmax;
  const char *doc;
} tbl_entry_t;

typedef int (*gfun_t)();
typedef void (*pfun_t)(int);

enum builtins: builtin_t { NIL, TEE, NOTHING, OPTIONAL, FEATURES, INITIALELEMENT, ELEMENTTYPE, TEST, COLONA, COLONB,
COLONC, BIT, AMPREST, LAMBDA, LET, LETSTAR, CLOSURE, PSTAR, QUOTE, DEFUN, DEFVAR, EQ, CAR, FIRST, CDR,
REST, NTH, AREF, CHAR, STRINGFN, PINMODE, DIGITALWRITE, ANALOGREAD, REGISTER, FORMAT, DEFCODE,
 };



#define BACKTRACESIZE 8


enum flag { PRINTREADABLY, RETURNFLAG, ESCAPE, EXITEDITOR, LIBRARYLOADED, NOESC, NOECHO, MUFFLEERRORS, BACKTRACE };
typedef uint16_t flags_t;



#define atom(x) (!consp(x))


#define improperp(x) (!listp(x))



#define ULISP_I2C1

//typedef FILE* File ;

#define HIGH    1
#define LOW     0


int getch(void);
char getche(void);
unsigned long int millis() ;


int modbacktrace (int n) ;
void printbacktrace () ;
void errorsub (symbol_t fname, const char *string) ;
void errorsym (symbol_t fname, const char *string, object *symbol) ;
void errorsym2 (symbol_t fname, const char *string) ;
void error (const char *string, object *symbol) ;
void error2 (const char *string) ;
void formaterr (object *formatstr, const char *string, uint8_t p) ;
void initworkspace () ;
object *myalloc () ;
void myfree (object *obj) ;
object *number (int n) ;
object *makefloat (sfloat_t f) ;
object *character (uint8_t c) ;
object *cons (object *arg1, object *arg2) ;
object *symbol (symbol_t name) ;
object *bsymbol (builtin_t name) ;
object *intern (symbol_t name) ;
bool eqsymbols (object *obj, char *buffer) ;
object *internlong (char *buffer) ;
object *stream (uint8_t streamtype, uint8_t address) ;
object *newstring () ;
object *features () ;
void markobject (object *obj) ;
void sweep () ;
void gc (object *form, object *env) ;
void movepointer (object *from, object *to) ;
uintptr_t compactimage (object **arg) ;
char *MakeFilename (object *arg, char *buffer) ;
void SDBegin() ;
void FSWrite32 (FILE *file, uint32_t data) ;
uint32_t FSRead32 (FILE *file) ;
void EpromWriteInt(int *addr, uintptr_t data) ;
int EpromReadInt (int *addr) ;
unsigned int saveimage (object *arg) ;
unsigned int loadimage (object *arg) ;
void autorunimage () ;
int tracing (symbol_t name) ;
void trace (symbol_t name) ;
void untrace (symbol_t name) ;
bool consp (object *x) ;
bool listp (object *x) ;
object *quote (object *arg) ;
builtin_t builtin (symbol_t name) ;
symbol_t sym (builtin_t x) ;
int8_t toradix40 (char ch) ;
char fromradix40 (char n) ;
uint32_t pack40 (char *buffer) ;
bool valid40 (char *buffer) ;
int8_t digitvalue (char d) ;
int checkinteger (object *obj) ;
int checkbitvalue (object *obj) ;
sfloat_t checkintfloat (object *obj) ;
int checkchar (object *obj) ;
object *checkstring (object *obj) ;
int isbuiltin (object *obj, builtin_t n) ;
bool builtinp (symbol_t name) ;
int checkkeyword (object *obj) ;
void checkargs (object *args) ;
bool eqlongsymbol (symbol_t sym1, symbol_t sym2) ;
bool eqsymbol (symbol_t sym1, symbol_t sym2) ;
bool eq (object *arg1, object *arg2) ;
bool equal (object *arg1, object *arg2) ;
int listlength (object *list) ;
object *checkarguments (object *args, int min, int max) ;
object *add_floats (object *args, sfloat_t fresult) ;
object *subtract_floats (object *args, sfloat_t fresult) ;
object *negate (object *arg) ;
object *multiply_floats (object *args, sfloat_t fresult) ;
object *divide_floats (object *args, sfloat_t fresult) ;
object *remmod (object *args, bool mod) ;
object *compare (object *args, bool lt, bool gt, bool eq) ;
int intpower (int base, int exp) ;
object *testargument (object *args) ;
object *delassoc (object *key, object **alist) ;
int nextpower2 (int n) ;
object *buildarray (int n, int s, object *def) ;
object *makearray (object *dims, object *def, bool bitp) ;
object **arrayref (object *array, int index, int size) ;
object **getarray (object *array, object *subs, object *env, int *bit) ;
void rslice (object *array, int size, int slice, object *dims, object *args) ;
object *readarray (int d, object *args) ;
object *readbitarray (gfun_t gfun) ;
void pslice (object *array, int size, int slice, object *dims, pfun_t pfun, bool bitp) ;
void printarray (object *array, pfun_t pfun) ;
void indent (uint8_t spaces, char ch, pfun_t pfun) ;
object *startstring () ;
object *princtostring (object *arg) ;
void buildstring (char ch, object** tail) ;
object *copystring (object *arg) ;
object *readstring (uint8_t delim, bool esc, gfun_t gfun) ;
int stringlength (object *form) ;
object **getcharplace (object *string, int n, int *shift) ;
uint8_t nthchar (object *string, int n) ;
int gstr () ;
void pstr (int c) ;
object *lispstring (char *s) ;
int stringcompare (object *args, bool lt, bool gt, bool eq) ;
object *documentation (object *arg, object *env) ;
object *apropos (object *arg, bool print) ;
char *cstring (object *form, char *buffer, int buflen) ;
object *iptostring (uint32_t ip) ;
uint32_t ipstring (object *form) ;
object *value (symbol_t n, object *env) ;
object *findpair (object *var, object *env) ;
bool boundp (object *var, object *env) ;
object *findvalue (object *var, object *env) ;
object *closure (int tc, symbol_t name, object *function, object *args, object **env) ;
object *apply (object *function, object *args, object *env) ;
object **place (object *args, object *env, int *bit, int *aray2_t) ;
object *carx (object *arg) ;
object *cdrx (object *arg) ;
object *cxxxr (object *args, uint8_t pattern) ;
object *mapcl (object *args, object *env, bool mapl) ;
void mapcarfun (object *result, object **tail) ;
void mapcanfun (object *result, object **tail) ;
object *mapcarcan (object *args, object *env, mapfun_t fun, bool maplist) ;
object *dobody (object *args, object *env, bool star) ;

inline int SDread () ;
inline int WiFiread () ;
void serialbegin (int address, int baud) ;
void serialend (int address) ;
gfun_t gstreamfun (object *args) ;
pfun_t pstreamfun (object *args) ;
void checkanalogread (int pin) ;
void checkanalogwrite (int pin) ;
void tone (int pin, int note) ;
void noTone (int pin) ;
void playnote (int pin, int note, int octave) ;
void nonote (int pin) ;
void doze (int secs) ;
void pcount (char c) ;
uint8_t atomwidth (object *obj) ;
uint8_t basewidth (object *obj, uint8_t base) ;
bool quoted (object *obj) ;
int subwidth (object *obj, int w) ;
int subwidthlist (object *form, int w) ;
void superprint (object *form, int lm, pfun_t pfun) ;
object *edit (object *fun) ;
object *sp_quote (object *args, object *env) ;
object *sp_or (object *args, object *env) ;
object *sp_defun (object *args, object *env) ;
object *sp_defvar (object *args, object *env) ;
object *sp_setq (object *args, object *env) ;
object *sp_loop (object *args, object *env) ;
object *sp_push (object *args, object *env) ;
object *sp_pop (object *args, object *env) ;
object *sp_incf (object *args, object *env) ;
object *sp_decf (object *args, object *env) ;
object *sp_setf (object *args, object *env) ;
object *sp_dolist (object *args, object *env) ;
object *sp_dotimes (object *args, object *env) ;
object *sp_do (object *args, object *env) ;
object *sp_dostar (object *args, object *env) ;
object *sp_trace (object *args, object *env) ;
object *sp_untrace (object *args, object *env) ;
object *sp_formillis (object *args, object *env) ;
object *sp_time (object *args, object *env) ;
object *sp_withoutputtostring (object *args, object *env) ;
object *sp_withserial (object *args, object *env) ;
object *sp_withi2c (object *args, object *env) ;
object *sp_withspi (object *args, object *env) ;
object *sp_withsdcard (object *args, object *env) ;
object *tf_progn (object *args, object *env) ;
object *tf_if (object *args, object *env) ;
object *tf_cond (object *args, object *env) ;
object *tf_when (object *args, object *env) ;
object *tf_unless (object *args, object *env) ;
object *tf_case (object *args, object *env) ;
object *tf_and (object *args, object *env) ;
object *fn_not (object *args, object *env) ;
object *fn_cons (object *args, object *env) ;
object *fn_atom (object *args, object *env) ;
object *fn_listp (object *args, object *env) ;
object *fn_consp (object *args, object *env) ;
object *fn_symbolp (object *args, object *env) ;
object *fn_arrayp (object *args, object *env) ;
object *fn_boundp (object *args, object *env) ;
object *fn_keywordp (object *args, object *env) ;
object *fn_setfn (object *args, object *env) ;
object *fn_streamp (object *args, object *env) ;
object *fn_eq (object *args, object *env) ;
object *fn_equal (object *args, object *env) ;
object *fn_car (object *args, object *env) ;
object *fn_cdr (object *args, object *env) ;
object *fn_caar (object *args, object *env) ;
object *fn_cadr (object *args, object *env) ;
object *fn_cdar (object *args, object *env) ;
object *fn_cddr (object *args, object *env) ;
object *fn_caaar (object *args, object *env) ;
object *fn_caadr (object *args, object *env) ;
object *fn_cadar (object *args, object *env) ;
object *fn_caddr (object *args, object *env) ;
object *fn_cdaar (object *args, object *env) ;
object *fn_cdadr (object *args, object *env) ;
object *fn_cddar (object *args, object *env) ;
object *fn_cdddr (object *args, object *env) ;
object *fn_length (object *args, object *env) ;
object *fn_arraydimensions (object *args, object *env) ;
object *fn_list (object *args, object *env) ;
object *fn_copylist (object *args, object *env) ;
object *fn_makearray (object *args, object *env) ;
object *fn_reverse (object *args, object *env) ;
object *fn_nth (object *args, object *env) ;
object *fn_aref (object *args, object *env) ;
object *fn_assoc (object *args, object *env) ;
object *fn_member (object *args, object *env) ;
object *fn_apply (object *args, object *env) ;
object *fn_funcall (object *args, object *env) ;
object *fn_append (object *args, object *env) ;
object *fn_mapc (object *args, object *env) ;
object *fn_mapl (object *args, object *env) ;
object *fn_mapcar (object *args, object *env) ;
object *fn_mapcan (object *args, object *env) ;
object *fn_maplist (object *args, object *env) ;
object *fn_mapcon (object *args, object *env) ;
object *fn_add (object *args, object *env) ;
object *fn_subtract (object *args, object *env) ;
object *fn_multiply (object *args, object *env) ;
object *fn_divide (object *args, object *env) ;
object *fn_mod (object *args, object *env) ;
object *fn_rem (object *args, object *env) ;
object *fn_oneplus (object *args, object *env) ;
object *fn_oneminus (object *args, object *env) ;
object *fn_abs (object *args, object *env) ;
object *fn_random (object *args, object *env) ;
object *fn_maxfn (object *args, object *env) ;
object *fn_minfn (object *args, object *env) ;
object *fn_noteq (object *args, object *env) ;
object *fn_numeq (object *args, object *env) ;
object *fn_less (object *args, object *env) ;
object *fn_lesseq (object *args, object *env) ;
object *fn_greater (object *args, object *env) ;
object *fn_greatereq (object *args, object *env) ;
object *fn_plusp (object *args, object *env) ;
object *fn_minusp (object *args, object *env) ;
object *fn_zerop (object *args, object *env) ;
object *fn_oddp (object *args, object *env) ;
object *fn_evenp (object *args, object *env) ;
object *fn_integerp (object *args, object *env) ;
object *fn_numberp (object *args, object *env) ;
object *fn_floatfn (object *args, object *env) ;
object *fn_floatp (object *args, object *env) ;
object *fn_sin (object *args, object *env) ;
object *fn_cos (object *args, object *env) ;
object *fn_tan (object *args, object *env) ;
object *fn_asin (object *args, object *env) ;
object *fn_acos (object *args, object *env) ;
object *fn_atan (object *args, object *env) ;
object *fn_sinh (object *args, object *env) ;
object *fn_cosh (object *args, object *env) ;
object *fn_tanh (object *args, object *env) ;
object *fn_exp (object *args, object *env) ;
object *fn_sqrt (object *args, object *env) ;
object *fn_log (object *args, object *env) ;
object *fn_expt (object *args, object *env) ;
object *fn_ceiling (object *args, object *env) ;
object *fn_floor (object *args, object *env) ;
object *fn_truncate (object *args, object *env) ;
object *fn_round (object *args, object *env) ;
object *fn_char (object *args, object *env) ;
object *fn_charcode (object *args, object *env) ;
object *fn_codechar (object *args, object *env) ;
object *fn_characterp (object *args, object *env) ;
object *fn_stringp (object *args, object *env) ;
object *fn_stringeq (object *args, object *env) ;
object *fn_stringless (object *args, object *env) ;
object *fn_stringgreater (object *args, object *env) ;
object *fn_stringnoteq (object *args, object *env) ;
object *fn_stringlesseq (object *args, object *env) ;
object *fn_stringgreatereq (object *args, object *env) ;
object *fn_sort (object *args, object *env) ;
object *fn_stringfn (object *args, object *env) ;
object *fn_concatenate (object *args, object *env) ;
object *fn_subseq (object *args, object *env) ;
object *fn_search (object *args, object *env) ;
object *fn_readfromstring (object *args, object *env) ;
object *fn_princtostring (object *args, object *env) ;
object *fn_prin1tostring (object *args, object *env) ;
object *fn_logand (object *args, object *env) ;
object *fn_logior (object *args, object *env) ;
object *fn_logxor (object *args, object *env) ;
object *fn_lognot (object *args, object *env) ;
object *fn_ash (object *args, object *env) ;
object *fn_logbitp (object *args, object *env) ;
object *fn_eval (object *args, object *env) ;
object *fn_return (object *args, object *env) ;
object *fn_globals (object *args, object *env) ;
object *fn_locals (object *args, object *env) ;
object *fn_makunbound (object *args, object *env) ;
object *fn_break (object *args, object *env) ;
object *fn_read (object *args, object *env) ;
object *fn_prin1 (object *args, object *env) ;
object *fn_print (object *args, object *env) ;
object *fn_princ (object *args, object *env) ;
object *fn_terpri (object *args, object *env) ;
object *fn_readbyte (object *args, object *env) ;
object *fn_readline (object *args, object *env) ;
object *fn_writebyte (object *args, object *env) ;
object *fn_writestring (object *args, object *env) ;
object *fn_writeline (object *args, object *env) ;
object *fn_restarti2c (object *args, object *env) ;
object *fn_gc (object *args, object *env) ;
object *fn_room (object *args, object *env) ;
object *fn_backtrace (object *args, object *env) ;
object *fn_saveimage (object *args, object *env) ;
object *fn_loadimage (object *args, object *env) ;
object *fn_cls (object *args, object *env) ;
object *fn_pinmode (object *args, object *env) ;
object *fn_digitalread (object *args, object *env) ;
object *fn_digitalwrite (object *args, object *env) ;
object *fn_analogread (object *args, object *env) ;
object *fn_analogreadresolution (object *args, object *env) ;
object *fn_analogwrite (object *args, object *env) ;
object *fn_delay (object *args, object *env) ;
object *fn_millis (object *args, object *env) ;
object *fn_sleep (object *args, object *env) ;
object *fn_note (object *args, object *env) ;
object *fn_register (object *args, object *env) ;
object *fn_edit (object *args, object *env) ;
object *fn_pprint (object *args, object *env) ;
object *fn_pprintall (object *args, object *env) ;
object *fn_format (object *args, object *env) ;
object *fn_require (object *args, object *env) ;
object *fn_listlibrary (object *args, object *env) ;
object *sp_help (object *args, object *env) ;
object *fn_documentation (object *args, object *env) ;
object *fn_apropos (object *args, object *env) ;
object *fn_aproposlist (object *args, object *env) ;
object *sp_unwindprotect (object *args, object *env) ;
object *sp_ignoreerrors (object *args, object *env) ;
object *sp_error (object *args, object *env) ;
object *fn_directory (object *args, object *env) ;
object *sp_withclient (object *args, object *env) ;
object *fn_available (object *args, object *env) ;
object *fn_wifiserver (object *args, object *env) ;
object *fn_wifisoftap (object *args, object *env) ;
object *fn_connected (object *args, object *env) ;
object *fn_wifilocalip (object *args, object *env) ;
object *fn_wificonnect (object *args, object *env) ;
object *sp_withgfx (object *args, object *env) ;
object *fn_drawpixel (object *args, object *env) ;
object *fn_drawline (object *args, object *env) ;
object *fn_drawrect (object *args, object *env) ;
object *fn_fillrect (object *args, object *env) ;
object *fn_drawcircle (object *args, object *env) ;
object *fn_fillcircle (object *args, object *env) ;
object *fn_drawroundrect (object *args, object *env) ;
object *fn_fillroundrect (object *args, object *env) ;
object *fn_drawtriangle (object *args, object *env) ;
object *fn_filltriangle (object *args, object *env) ;
object *fn_drawchar (object *args, object *env) ;
object *fn_setcursor (object *args, object *env) ;
object *fn_settextcolor (object *args, object *env) ;
object *fn_settextsize (object *args, object *env) ;
object *fn_settextwrap (object *args, object *env) ;
object *fn_fillscreen (object *args, object *env) ;
object *fn_setrotation (object *args, object *env) ;
object *fn_invertdisplay (object *args, object *env) ;
const tbl_entry_t* table (int n) ;
unsigned int tablesize (int n) ;
builtin_t lookupbuiltin (char* c) ;
intptr_t lookupfn (builtin_t name) ;
uint8_t getminmax (builtin_t name) ;
void checkminmax (builtin_t name, int nargs) ;
char *lookupdoc (builtin_t name) ;
bool findsubstring (char *part, builtin_t name) ;
void testescape () ;
bool colonp (symbol_t name) ;
bool keywordp (object *obj) ;
void backtrace (symbol_t name) ;
object *eval (object *form, object *env) ;
void pserial (int c) ;
void pcharacter (uint8_t c, pfun_t pfun) ;
void pstring (char *s, pfun_t pfun) ;
void plispstring (object *form, pfun_t pfun) ;
void plispstr (symbol_t name, pfun_t pfun) ;
void printstring (object *form, pfun_t pfun) ;
void pbuiltin (builtin_t name, pfun_t pfun) ;
void pradix40 (symbol_t name, pfun_t pfun) ;
void printsymbol (object *form, pfun_t pfun) ;
void psymbol (symbol_t name, pfun_t pfun) ;
void pfstring (const char *s, pfun_t pfun) ;
void pint (int i, pfun_t pfun) ;
void pintbase (uint32_t i, uint8_t base, pfun_t pfun) ;
void pmantissa (float f, pfun_t pfun) ;
void pfloat (float f, pfun_t pfun) ;
void pln (pfun_t pfun) ;
void pfl (pfun_t pfun) ;
void plist (object *form, pfun_t pfun) ;
void pstream (object *form, pfun_t pfun) ;
void printobject (object *form, pfun_t pfun) ;
void prin1object (object *form, pfun_t pfun) ;
int glibrary () ;
void loadfromlibrary (object *env) ;
void esc (int p, char c) ;
void hilight (char c) ;
void Highlight (int p, int wp, uint8_t invert) ;
void processkey (char c) ;
int gserial () ;
object *nextitem (gfun_t gfun) ;
object *readrest (gfun_t gfun) ;
object *read0 (gfun_t gfun) ;
void initenv () ;
void initgfx () ;
void setup () ;
void repl (object *env) ;
void loop () ;
void ulisperror () ;
void printhex4 (int i, pfun_t pfun) ;
void delay(int ms);
//#define usleep(a)   Delay_Us(a) ;
//#define sleep(a)   Delay_Ms(a) ;

#endif // __ULISP_H
