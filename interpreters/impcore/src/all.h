/* {\Tt all.h} for \impcore S290 */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __GNUC__
#define __noreturn __attribute__((noreturn))
#else
#define __noreturn
#endif


/* type definitions for \impcore 44a */
typedef int32_t Value;
typedef struct Valuelist *Valuelist;     // list of Value
/* type definitions for \impcore 44b */
typedef struct Funclist *Funclist; // list of Func
/* type definitions for \impcore 44f */
typedef struct Valenv *Valenv;
typedef struct Funenv *Funenv;
/* type definitions for \impcore S288b */
typedef struct UnitTestlist *UnitTestlist; // list of UnitTest
/* type definitions for \impcore S288c */
typedef struct Explist *Explist; // list of Exp
/* type definitions for \impcore (generated by a script) */
typedef struct Exp *Exp;
typedef enum { LITERAL, VAR, SET, IFX, WHILEX, BEGIN, APPLY } Expalt;

/* type definitions for \impcore (generated by a script) */
typedef struct Userfun Userfun; 
typedef struct Def *Def;
typedef enum { VAL, EXP, DEFINE } Defalt; 
/* type definitions for \impcore (generated by a script) */
typedef struct XDef *XDef;
typedef enum { DEF, USE, TEST } XDefalt; 
typedef struct UnitTest *UnitTest;
typedef enum { CHECK_EXPECT, CHECK_ASSERT, CHECK_ERROR } UnitTestalt;

/* type definitions for \impcore (generated by a script) */
typedef struct Func Func;
typedef enum { USERDEF, PRIMITIVE } Funcalt; 
/* shared type definitions 43b */
typedef struct Name *Name;
typedef struct Namelist *Namelist;   // list of Name
/* shared type definitions S206b */
typedef struct ParserState *ParserState;
typedef struct ParsingContext *ParsingContext;
/* shared type definitions S207c */
typedef enum ParserResult {
  PARSED,            /* some input was parsed without any errors */
  INPUT_EXHAUSTED,   /* there aren't enough inputs */
  INPUT_LEFTOVER,    /* there are too many inputs */
  BAD_INPUT,         /* an input wasn't what it should have been */
  STOP_PARSING       /* all the inputs have been parsed; it's time to stop */
} ParserResult;
/* shared type definitions S207d */
typedef ParserResult (*ShiftFun)(ParserState);
/* shared type definitions S211b */
typedef struct ParserRow *ParserTable;
/* shared type definitions S217a */
enum Sugar {
  CAND, COR,    /* short-circuit Boolean operators */

  WHILESTAR, DO_WHILE, FOR,     /* bonus loop forms */

  WHEN, UNLESS,       /* single-sided conditionals */

  RECORD,             /* record-type definition */

  COND                /* McCarthy's conditional from Lisp */

};
/* shared type definitions S178a */
typedef struct Linestream *Linestream;
/* shared type definitions S181b */
typedef struct Parlist *Parlist; /* list of Par */
/* shared type definitions S181c */
typedef struct Parstream *Parstream;
/* shared type definitions S186d */
typedef struct Printbuf *Printbuf;
/* shared type definitions S189b */
/* definition of [[va_list_box]] S189c */
typedef struct va_list_box {
  va_list ap;
} va_list_box;
typedef void Printer(Printbuf output, va_list_box *args);
/* shared type definitions S288d */
typedef struct XDefstream *XDefstream;
/* shared type definitions S288g */
typedef enum Prompts { NO_PROMPTS, STD_PROMPTS } Prompts;
/* shared type definitions S289b */
typedef enum Echo { NO_ECHOES, ECHOES } Echo;
/* shared type definitions S289d */
typedef struct Sourceloc *Sourceloc;
/* shared type definitions S289e */
typedef enum ErrorFormat { WITH_LOCATIONS, WITHOUT_LOCATIONS } ErrorFormat;
/* shared type definitions S295b */
typedef enum TestResult { TEST_PASSED, TEST_FAILED } TestResult;
/* shared type definitions (generated by a script) */
typedef struct Par *Par;
typedef enum { ATOM, LIST } Paralt; 

/* structure definitions for \impcore S204b */
struct Component {
    Exp exp;
    Explist exps;
    Name name;
    Namelist names;
};
/* structure definitions for \impcore (generated by a script) */
struct Exp {
    Expalt alt;
    union {
        Value literal;
        Name var;
        struct { Name name; Exp exp; } set;
        struct { Exp cond; Exp truex; Exp falsex; } ifx;
        struct { Exp cond; Exp exp; } whilex;
        Explist begin;
        struct { Name name; Explist actuals; } apply;
    } ;
};

/* structure definitions for \impcore (generated by a script) */
struct Userfun { Namelist formals; Exp body; }; 
struct Def {
    Defalt alt;
    union {
        struct { Name name; Exp exp; } val;
        Exp exp;
        struct { Name name; Userfun userfun; } define;
    } ;
};

/* structure definitions for \impcore (generated by a script) */
struct XDef { XDefalt alt; union { Def def; Name use; UnitTest test; } ; };

struct UnitTest {
    UnitTestalt alt;
    union {
        struct { Exp check; Exp expect; } check_expect;
        Exp check_assert;
        Exp check_error;
    } ;
};

/* structure definitions for \impcore (generated by a script) */
struct Func { Funcalt alt; union { Userfun userdef; Name primitive; } ; }; 
/* structure definitions for \impcore (generated by a script) */
struct Parlist {
   Par hd;
   struct Parlist *tl;
};

struct Namelist {
   Name hd;
   struct Namelist *tl;
};

struct Valuelist {
   Value hd;
   struct Valuelist *tl;
};

struct Funclist {
   Func hd;
   struct Funclist *tl;
};

struct UnitTestlist {
   UnitTest hd;
   struct UnitTestlist *tl;
};

struct Explist {
   Exp hd;
   struct Explist *tl;
};

/* shared structure definitions S206a */
#define MAXCOMPS 4 /* max # of components in any syntactic form */
struct ParserState {
    int nparsed;           /* number of components parsed so far */
    struct Component components[MAXCOMPS];  /* those components */
    Parlist input;         /* the part of the input not yet parsed */

    struct ParsingContext {   /* context of this parse */
        Par par;       /* the original thing we are parsing */
        struct Sourceloc {
            int line;                /* current line number */
            const char *sourcename;  /* where the line came from */
        } *source;
        Name name;     /* a keyword, or name of a function being defined */
    } context;
};
/* shared structure definitions S210d */
struct ParserRow {
    const char *keyword;
    int code;
    ShiftFun *shifts;  /* points to array of shift functions */
};
/* shared structure definitions S178d */
struct Linestream {
    char *buf;               /* holds the last line read */
    int bufsize;                /* size of buf */

    struct Sourceloc source; /* where the last line came from */
    FILE *fin;               /* non-NULL if filelines */
    const char *s;           /* non-NULL if stringlines */
};
/* shared structure definitions (generated by a script) */
struct Par { Paralt alt; union { Name atom; Parlist list; } ; }; 

/* function prototypes for \impcore 45a */
Valenv mkValenv(Namelist vars, Valuelist vals);
Funenv mkFunenv(Namelist vars, Funclist  defs);
/* function prototypes for \impcore 45b */
Value fetchval(Name name, Valenv env);
Func  fetchfun(Name name, Funenv env);
/* function prototypes for \impcore 45c */
bool isvalbound(Name name, Valenv env);
bool isfunbound(Name name, Funenv env);
/* function prototypes for \impcore 45d */
void bindval(Name name, Value val, Valenv env);
void bindfun(Name name, Func  fun, Funenv env);
/* function prototypes for \impcore 45e */
Value eval   (Exp e, Valenv globals, Funenv functions, Valenv formals);
void  evaldef(Def d, Valenv globals, Funenv functions, Echo echo_level);
/* function prototypes for \impcore S289a */
void readevalprint(XDefstream s, Valenv globals, Funenv functions, Echo
                                                                    echo_level);
/* function prototypes for \impcore S291b */
void process_tests(UnitTestlist tests, Valenv globals, Funenv functions);
/* function prototypes for \impcore S294d */
int number_of_good_tests(UnitTestlist tests, Valenv globals, Funenv functions);
/* function prototypes for \impcore S295c */
TestResult test_result(UnitTest t, Valenv globals, Funenv functions);
/* function prototypes for \impcore (generated by a script) */
Exp mkLiteral(Value literal);
Exp mkVar(Name var);
Exp mkSet(Name name, Exp exp);
Exp mkIfx(Exp cond, Exp truex, Exp falsex);
Exp mkWhilex(Exp cond, Exp exp);
Exp mkBegin(Explist begin);
Exp mkApply(Name name, Explist actuals);
struct Exp mkLiteralStruct(Value literal);
struct Exp mkVarStruct(Name var);
struct Exp mkSetStruct(Name name, Exp exp);
struct Exp mkIfxStruct(Exp cond, Exp truex, Exp falsex);
struct Exp mkWhilexStruct(Exp cond, Exp exp);
struct Exp mkBeginStruct(Explist begin);
struct Exp mkApplyStruct(Name name, Explist actuals);
/* function prototypes for \impcore S287 */
Userfun mkUserfun(Namelist formals, Exp body);
Def mkVal(Name name, Exp exp);
Def mkExp(Exp exp);
Def mkDefine(Name name, Userfun userfun);
struct Def mkValStruct(Name name, Exp exp);
struct Def mkExpStruct(Exp exp);
struct Def mkDefineStruct(Name name, Userfun userfun);
/* function prototypes for \impcore (generated by a script) */
XDef mkDef(Def def);
XDef mkUse(Name use);
XDef mkTest(UnitTest test);
struct XDef mkDefStruct(Def def);
struct XDef mkUseStruct(Name use);
struct XDef mkTestStruct(UnitTest test);
UnitTest mkCheckExpect(Exp check, Exp expect);
UnitTest mkCheckAssert(Exp check_assert);
UnitTest mkCheckError(Exp check_error);
struct UnitTest mkCheckExpectStruct(Exp check, Exp expect);
struct UnitTest mkCheckAssertStruct(Exp check_assert);
struct UnitTest mkCheckErrorStruct(Exp check_error);
/* function prototypes for \impcore 44e */
Func mkUserdef(Userfun userdef);
Func mkPrimitive(Name primitive);
/* function prototypes for \impcore (generated by a script) */
int     lengthPL(Parlist ps);
Par     nthPL   (Parlist ps, unsigned n);
Parlist mkPL    (Par p, Parlist ps);
Parlist popPL   (Parlist ps);
Printer printparlist;

int      lengthNL(Namelist ns);
Name     nthNL   (Namelist ns, unsigned n);
Namelist mkNL    (Name n, Namelist ns);
Namelist popNL   (Namelist ns);
Printer  printnamelist;

int       lengthVL(Valuelist vs);
Value     nthVL   (Valuelist vs, unsigned n);
Valuelist mkVL    (Value v, Valuelist vs);
Valuelist popVL   (Valuelist vs);
Printer   printvaluelist;

int      lengthFL(Funclist fs);
Func     nthFL   (Funclist fs, unsigned n);
Funclist mkFL    (Func f, Funclist fs);
Funclist popFL   (Funclist fs);
Printer  printfunclist;

int          lengthUL(UnitTestlist us);
UnitTest     nthUL   (UnitTestlist us, unsigned n);
UnitTestlist mkUL    (UnitTest u, UnitTestlist us);
UnitTestlist popUL   (UnitTestlist us);
Printer      printunittestlist;

int     lengthEL(Explist es);
Exp     nthEL   (Explist es, unsigned n);
Explist mkEL    (Exp e, Explist es);
Explist popEL   (Explist es);
Printer printexplist;

/* shared function prototypes 43c */
Name strtoname(const char *s);
const char *nametostr(Name x);
/* shared function prototypes 46c */
void print (const char *fmt, ...);  // print to standard output
void fprint(FILE *output, const char *fmt, ...);  // print to given file
/* shared function prototypes 47 */
__noreturn // OMIT
void runerror (const char *fmt, ...);
extern jmp_buf errorjmp;        // longjmp here on error
/* shared function prototypes 48a */
__noreturn // OMIT
void synerror (Sourceloc src, const char *fmt, ...);
/* shared function prototypes 48b */
void checkargc(Exp e, int expected, int actual);
/* shared function prototypes S202a */
Exp  parseexp (Par p, Sourceloc source);
XDef parsexdef(Par p, Sourceloc source);
/* shared function prototypes S202b */
Exp exp_of_atom(Sourceloc loc, Name atom);
/* shared function prototypes S204c */
Exp  reduce_to_exp (int alt, struct Component *components);
XDef reduce_to_xdef(int alt, struct Component *components);
/* shared function prototypes S207b */
struct ParserState mkParserState(Par p, Sourceloc source);
/* shared function prototypes S207e */
ParserResult sExp     (ParserState state);  /* shift 1 input into Exp */
ParserResult sExps    (ParserState state);  /* shift all inputs into Explist */
ParserResult sName    (ParserState state);  /* shift 1 input into Name */
ParserResult sNamelist(ParserState state);  /* shift 1 input into Namelist */
/* shared function prototypes S208b */
void halfshift(ParserState state); /* advance input, check for room in output */
/* shared function prototypes S208e */
Explist parseexplist(Parlist p, Sourceloc source);
/* shared function prototypes S208g */
Name parsename(Par p, ParsingContext context);
/* shared function prototypes S209d */
ParserResult stop(ParserState state);
/* shared function prototypes S209f */
ParserResult setcontextname(ParserState state);
/* shared function prototypes S210c */
ParserResult sLocals(ParserState state);  // shift locals if (locals x y z ...)
/* shared function prototypes S211a */
void rowparse(struct ParserRow *table, ParserState s);
__noreturn /* OMIT */
void usage_error(int alt, ParserResult r, ParsingContext context);
/* shared function prototypes S211d */
struct ParserRow *tableparse(ParserState state, ParserTable t);
/* shared function prototypes S214b */
ParserResult use_exp_parser(ParserState state);
/* shared function prototypes S217b */
int code_of_name(Name n);
/* shared function prototypes S217c */
void check_exp_duplicates(Sourceloc source, Exp e);
void check_def_duplicates(Sourceloc source, Def d);
/* shared function prototypes S178b */
char *getline_(Linestream r, const char *prompt);
/* shared function prototypes S178c */
Linestream stringlines(const char *stringname, const char *s);
Linestream filelines  (const char *filename,   FILE *fin);
/* shared function prototypes S181d */
Parstream parstream(Linestream lines, Prompts prompts);
Par       getpar   (Parstream r);
Sourceloc parsource(Parstream pars);
/* shared function prototypes S181e */
extern bool read_tick_as_quote;
/* shared function prototypes S186e */
Printbuf printbuf(void);
void freebuf(Printbuf *);
/* shared function prototypes S186f */
void bufput(Printbuf, char);
void bufputs(Printbuf, const char*);
void bufreset(Printbuf);
/* shared function prototypes S186g */
char *bufcopy(Printbuf);
void fwritebuf(Printbuf buf, FILE *output);
/* shared function prototypes S188f */
void print (const char *fmt, ...);                /* print to standard output */
void fprint(FILE *output, const char *fmt, ...);     /* print to given file */
void bprint(Printbuf output, const char *fmt, ...);  /* print to given buffer */
/* shared function prototypes S189a */
void installprinter(unsigned char specifier, Printer *take_and_print);
/* shared function prototypes S189d */
void vbprint(Printbuf output, const char *fmt, va_list_box *box);
/* shared function prototypes S191c */
Printer printpercent, printstring, printdecimal, printchar, printname,
                                                                   printpointer;
/* shared function prototypes S192d */
Printer printpar;
/* shared function prototypes S193a */
typedef enum ErrorMode { NORMAL, TESTING } ErrorMode;
void set_error_mode(ErrorMode mode);
extern jmp_buf testjmp;    /* if error occurs during a test, longjmp here */
extern Printbuf errorbuf;  /* if error occurs during a test, message is here */
/* shared function prototypes S195b */
__noreturn // OMIT
void othererror (const char *fmt, ...);
/* shared function prototypes S196a */
Name duplicatename(Namelist names);
/* shared function prototypes S197a */
extern int  checkoverflow(int limit);
extern void reset_overflow_check(void);
/* shared function prototypes S198b */
extern void checkarith(char operation, int32_t n, int32_t m, int precision);
/* shared function prototypes S199a */
void fprint_utf8(FILE *output, unsigned code_point);
void print_utf8 (unsigned u);
/* shared function prototypes S288e */
XDef getxdef(XDefstream xdefs);
/* shared function prototypes S288f */
XDefstream stringxdefs(const char *stringname, const char *input);
XDefstream filexdefs  (const char *filename, FILE *input, Prompts prompts);
/* shared function prototypes S289c */
void installprinter(unsigned char c, Printer *take_and_print);
/* shared function prototypes S289f */
void set_toplevel_error_format(ErrorFormat format);
/* shared function prototypes S294e */
void report_test_results(int npassed, int ntests);
/* shared function prototypes S297f */
Printer printexp, printdef, printvalue, printfun;
/* shared function prototypes (generated by a script) */
Par mkAtom(Name atom);
Par mkList(Parlist list);
struct Par mkAtomStruct(Name atom);
struct Par mkListStruct(Parlist list);

/* macro definitions used in parsing S205c */
#define ANEXP(ALT)  (  0+(ALT))
#define ADEF(ALT)   (100+(ALT))
#define ATEST(ALT)  (200+(ALT))
#define ANXDEF(ALT) (300+(ALT))
#define ALET(ALT)   (400+(ALT))
#define SUGAR(CODE) (500+(CODE))
#define LATER       1000
#define EXERCISE    1001
/* declarations of global variables used in lexical analysis and parsing S211h */
extern struct ParserRow exptable[];
extern struct ParserRow xdeftable[];
/* declarations of global variables used in lexical analysis and parsing S215b */
extern struct Usage {
    int code;
                         /* codes for form in reduce_to_exp or reduce_to_xdef */
    const char *expected;  /* shows the expected usage of the identified form */
} usage_table[];
