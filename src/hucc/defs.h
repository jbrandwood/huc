/*	File defs.h: 2.1 (83/03/21,02:07:20) */

#ifndef INCLUDE_DEFS_H
#define INCLUDE_DEFS_H

/*
 *	INTSIZE is the size of an integer in the target machine
 *	BYTEOFF is the offset of an byte within an integer on the
 *		target machine. (ie: 8080,pdp11 = 0, 6809 = 1,
 *		360 = 3)
 *	This compiler assumes that an integer is the SAME length as
 *	a pointer - in fact, the compiler uses INTSIZE for both.
 */

#define INTSIZE 2
#define BYTEOFF 0

/* pseudo instruction arg types */
#define T_NOP           -1
#define T_VALUE          1
#define T_LABEL          2
#define T_SYMBOL         3
#define T_PTR            4
#define T_STACK          5
#define T_STRING         6
#define T_LIB            7
#define T_SIZE           8
#define T_BANK           9
#define T_VRAM          10
#define T_PAL           11
#define T_LITERAL       12

/* basic pseudo instructions */
#define I_LDB           1
#define I_LDBP          2
#define I_LDW           3
#define I_LDWI          4
#define I_LDWP          5
#define I_STB           6
#define I_STBPS         7
#define I_STW           8
#define I_STWPS         9
#define I_ADDWI         10
#define I_ADDWS         11
#define I_MODSP         12
#define I_SUBWI         13
#define I_SUBWS         14
#define I_ORWS          15
#define I_EORWS         16
#define I_ANDWS         17
#define I_ASLW          18
#define I_ASLWS         19
#define I_ASRW          20
#define I_COMW          21
#define I_NEGW          22
#define I_EXTW          23
#define I_BOOLW         24
#define I_NOTW          25
#define I_JMP           26
#define I_JSR           27
#define I_ENTER         28
#define I_LEAVE         29
#define I_CALL          30
#define I_MACRO         31
#define I_CALLP         32
#define I_PUSHW         33
#define I_POPW          34
#define I_TSTW          35
#define I_BRA           36
#define I_BEQ           37
#define I_BNE           38
#define I_BANK          39
#define I_OFFSET        40
#define I_FARPTR        41
#define I_FARPTR_I      42
#define I_FARPTR_GET    43
#define I_FGETB         44
#define I_FGETW         45
#define I_INCW          46
#define I_ANDWI         47
#define I_ORWI          48
#define I_ADDW          49
#define I_SUBW          50
#define I_LDUB          51
#define I_LDUBP         52
#define I_FGETUB        53
#define I_ADDBS         54
#define I_ADDBI         55
#define I_LABEL         56
#define I_STWIP         57
#define I_STBIP         58
#define I_MULWI         59
#define I_STWI          60
#define I_STBI          61
#define I_ASLWI         62
#define I_LSRWI         63
#define I_ASRWI         64
#define I_LDYB          65
#define I_LDBY          66
#define I_ADDB          67
#define I_ADDUB         68
#define I_ADDBI_P       69
#define I_INCB          70
#define I_STWZ          71
#define I_STBZ          72
#define I_CMPWI_EQ      73
#define I_CMPWI_NE      74
#define I_STBP          75
#define I_STWP          76
#define I_EORWI         77
#define I_SPUSHW        78 /* push and pop on the hw-stack */
#define I_SPUSHB        79 /* to temporarily save data */
#define I_SPOPW         80
#define I_SPOPB         81
#define I_EXTUW         82
#define I_DEF           83
#define I_SEI           84
#define I_CLI           85
#define I_SAVESP        86
#define I_LOADSP        87
#define I_GETACC        88

/* optimized pseudo instructions */
#define X_MASK          0xFFFF0
#define X_LDB           0x10000
#define X_LDB_S         0x10001
#define X_LDB_P         0x10002
#define X_LDW_S         0x10011
#define X_LDD_I         0x10100
#define X_LDD_B         0x10101
#define X_LDD_W         0x10102
#define X_LDD_S_B       0x10105
#define X_LDD_S_W       0x10106
#define X_LEA_S         0x10021
#define X_PEA           0x10030
#define X_PEA_S         0x10031
#define X_PUSHW_A       0x10040	/* XXX: unused? */
#define X_STB_S         0x10051
#define X_STW_S         0x10061
#define X_STBI_S        0x10071
#define X_STWI_S        0x10081
#define X_ADDW_S        0x10091
#define X_INCW_S        0x100A1
#define X_DECW_S        0x100B1
#define X_LDUB          0x10003
#define X_LDUB_S        0x10004
#define X_LDUB_P        0x10005
#define X_ADDB_S        0x100C1
#define X_ADDUB_S       0x100D1
#define X_INCB_S        0x100E1

#define FOREVER for (;;)
#define FALSE   0
#define TRUE    1
#define NO      0
#define YES     1

/* miscellaneous */

#define EOS     0
#define EOL     10
#define BKSP    8
#define CR      13
#define FFEED   12
#define TAB     9

#define FILENAMESIZE    256

/* symbol table parameters */

/* old values, too restrictive
 * #define	SYMSIZ	14
 * #define	SYMTBSZ	32768
 * #define	NUMGLBS	1500
 */
#define SYMTBSZ 4096
#define NUMGLBS 2048

#define STARTGLB        symtab
#define ENDGLB  (STARTGLB + NUMGLBS)
#define STARTLOC        (ENDGLB + 1)
#define ENDLOC  (symtab + SYMTBSZ - 1)

/* symbol table entry format */
/* N.B. nasty hack to allow space beyond NAMEMAX (see "copysym") */

#define NAMESIZE	48
#define NAMEMAX		47
#define NAMEALLOC	64

struct symbol {
	char name[NAMEALLOC];
	char ident;
	char type;
	char storage;
	char far;
	short offset;
	short tagidx;
	int size;
	int ptr_order;
};

typedef struct symbol SYMBOL;

#define NUMTAG  64

struct tag_symbol {
	char name[NAMESIZE];	// structure tag name
	int size;		// size of struct in bytes
	int member_idx;		// index of first member
	int number_of_members;	// number of tag members
};
#define TAG_SYMBOL struct tag_symbol

#define NULL_TAG 0

// Define the structure member table parameters
#define NUMMEMB         256

/* possible entries for "ident" */

#define VARIABLE        1
#define ARRAY   2
#define POINTER 3
#define FUNCTION        4

/* possible entries for "type" */

#define CCHAR   1
#define CINT    2
#define CVOID   3
#define CSTRUCT 4
#define CENUM   5
#define CSIGNED 0
#define CUNSIGNED 8
#define CUINT (CINT | CUNSIGNED)
#define CUCHAR (CCHAR | CUNSIGNED)

/* possible entries for storage */

#define PUBLIC  1
#define AUTO    2
#define EXTERN  3

#define STATIC  4
#define LSTATIC 5
#define DEFAUTO 6
#define CONST   7

#define WRITTEN 128

/* "do"/"for"/"while"/"switch" statement stack */

#define WSSIZ   7
#define WSTABSZ (WSSIZ * 16)
#define WSMAX   ws + WSTABSZ - WSSIZ

/* entry offsets in "do"/"for"/"while"/"switch" stack */

#define WSSYM   0
#define WSSP    1
#define WSTYP   2
#define WSCASEP 3
#define WSTEST  3
#define WSINCR  4
#define WSDEF   4
#define WSBODY  5
#define WSTAB   5
#define WSEXIT  6

/* possible entries for "wstyp" */

#define WSWHILE 0
#define WSFOR   1
#define WSDO    2
#define WSSWITCH        3

/* "switch" label stack */

#define SWSTSZ  256

/* literal pool */

#define LITABSZ 8192
#define LITMAX  LITABSZ - 1

/* input string */

#define LITMAX2 LITABSZ - 1

/* input line */

#define LINESIZE        384
#define LINEMAX (LINESIZE - 1)
#define MPMAX   LINEMAX

/* macro (define) pool */

#define MACQSIZE        16384
#define MACMAX  (MACQSIZE - 1)

struct macro {
	char *name;
	char **args;
	int argc;
	struct {
		int arg;
		int pos;
	} *argpos;
	char *def;
};

/* "include" stack */

#define INCLSIZ 3

/* statement types (tokens) */

#define STIF    1
#define STWHILE 2
#define STRETURN        3
#define STBREAK 4
#define STCONT  5
#define STASM   6
#define STEXP   7
#define STDO    8
#define STFOR   9
#define STSWITCH        10
#define STGOTO  11

/* pseudo instruction structure */

typedef struct {
	int code;
	int type;
	intptr_t data;
	int imm_type;
	intptr_t imm_data;
	char *arg[3];
	SYMBOL *sym;
} INS;

/* constant array struct */

#define MAX_CONST        1024
#define MAX_CONST_VALUE  8192
#define MAX_CONST_DATA  65536

struct const_array {
	SYMBOL *sym;
	int typ;
	int size;
	int data;
};

/* fastcall func struct */

#define MAX_FASTCALL_ARGS 16
#define FASTCALL_EXTRA    0x01  // bitmask values
#define FASTCALL_XSAFE    0x02  // bitmask values
#define FASTCALL_NOP      0x04  // bitmask values
#define FASTCALL_MACRO    0x08  // bitmask values

struct fastcall {
	struct fastcall *next;
	char fname[NAMESIZE];
	int nargs;
	int flags;
	char argtype[MAX_FASTCALL_ARGS];
	char argname[MAX_FASTCALL_ARGS][NAMESIZE];
};

// initialisation of global variables
#define INIT_TYPE    NAMESIZE
#define INIT_LENGTH  NAMESIZE + 1
#define INITIALS_SIZE 5 * 1024

struct initials_table {
	char name[NAMESIZE];	// symbol name
	int type;		// type
	int dim;		// length of data (possibly an array)
	int data_len;		// index of tag or zero
};
#define INITIALS struct initials_table

SYMBOL *find_member (TAG_SYMBOL *tag, char *sname);

struct lvalue {
	SYMBOL *symbol;
	int indirect;
	int ptr_type;
	SYMBOL *symbol2;
	int value;
	TAG_SYMBOL *tagsym;
	int ptr_order;
	int type;
};
#define LVALUE struct lvalue

#define W_GENERAL 1

struct type {
	int type;
	int ident;
	int ptr_order;
	int otag;
	int flags;
	char sname[NAMESIZE];
};
#define F_REGISTER 1
#define F_CONST 2
#define F_VOLATILE 4
#define F_STRUCT 8	/* set if CSTRUCT is struct, not union */

struct clabel {
	char name[NAMESIZE];
	int stkp;
	int label;
};

struct enum_s {
	char name[NAMESIZE];
	int value;
};
struct enum_type {
	char name[NAMESIZE];
	int start;
	int base;
};

#endif
