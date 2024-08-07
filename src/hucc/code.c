/*	File code.c: 2.2 (84/08/31,10:05:13) */
/*% cc -O -c %
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "defs.h"
#include "data.h"
#include "code.h"
#include "error.h"
#include "function.h"
#include "io.h"
#include "main.h"
#include "optimize.h"

/* locals */
int segment;

/* externs */
extern int arg_stack_flag;

/*
 *	print all assembler info before any code is generated
 *
 */
void gdata (void)
{
	if (segment == 1) {
		segment = 0;
		ol(".bss");
	}
}

void gtext (void)
{
	if (segment == 0) {
		segment = 1;
		ol(".code");
	}
}

void header (void)
{
	time_t today;

	outstr("; Small-C HuC6280 (1997-Nov-08)\n");
	outstr("; became HuC      (2000-Feb-22)\n");
	outstr("; became HuCC     (2024-May-01)\n");
	outstr(";\n");
	outstr("; This file generated by ");
	outstr(HUC_VERSION);
	outstr("\n");
	outstr("; on ");
	time(&today);
	outstr(ctime(&today));
	outstr(";\n");
	outstr("\n");
	outstr("HUC\t\t=\t1\n");
	outstr("HUCC\t\t=\t1\n");
	/* Reserve space for further global definitions. */
	output_globdef = ftell(output);
	outstr("                                                                           ");
	nl();
}

void asmdefines (void)
{
	outstr(asmdefs);
}

void inc_startup (void)
{
	if (startup_incl == 0) {
		startup_incl = 1;

		nl();
		outstr("\t\tinclude\t\"hucc.asm\"\n");
		outstr("\t\t.data\n");
		outstr("\t\t.bank\tDATA_BANK\n\n");
		gtext();
		nl();
	}
}

/*
 *	print pseudo-op  to define a byte
 *
 */
void defbyte (void)
{
	ot(".db\t");
}

/*
 *	print pseudo-op to define storage
 *
 */
void defstorage (void)
{
	ot(".ds\t");
}

/*
 *	print pseudo-op to define a word
 *
 */
void defword (void)
{
	ot(".dw\t");
}

/*
 *	output instructions
 *
 */
void out_ins (int code, int type, intptr_t data)
{
	INS tmp;

	memset(&tmp, 0, sizeof(INS));

	tmp.code = code;
	tmp.type = type;
	tmp.data = data;
	gen_ins(&tmp);
}

void out_ins_ex (int code, int type, intptr_t data, int imm_type, intptr_t imm_data)
{
	INS tmp;

	memset(&tmp, 0, sizeof(INS));

	tmp.code = code;
	tmp.type = type;
	tmp.data = data;
	tmp.imm_type = imm_type;
	tmp.imm_data = imm_data;
	gen_ins(&tmp);
}

void out_ins_sym (int code, int type, intptr_t data, SYMBOL *sym)
{
	INS tmp;

	memset(&tmp, 0, sizeof(INS));

	tmp.code = code;
	tmp.type = type;
	tmp.data = data;
	tmp.sym = sym;
	gen_ins(&tmp);
}

void gen_ins (INS *tmp)
{
	if (optimize)
		push_ins(tmp);
	else {
		if (arg_stack_flag)
			arg_push_ins(tmp);
		else
			gen_code(tmp);
	}
}

static void out_type (int type, intptr_t data)
{
	switch (type) {
	case T_VALUE:
		outdec((int)data);
		break;
	case T_LABEL:
		outlabel((int)data);
		break;
	case T_SYMBOL:
		outsymbol((char *)data);
		break;
	case T_LITERAL:
		outstr((const char *)data);
		break;
	case T_STRING:
		outconst(litlab);
		outbyte('+');
		outdec((int)data);
		break;
	case T_BANK:
		outstr("BANK(");
		outstr((const char *)data);
		outstr(")");
		break;
	case T_VRAM:
		outstr("VRAM(");
		outstr((const char *)data);
		outstr(")");
		break;
	case T_PAL:
		outstr("PAL(");
		outstr((const char *)data);
		outstr(")");
		break;
	}
}

static void out_addr (int type, intptr_t data)
{
	switch (type) {
	case T_LABEL:
		outlabel((int)data);
		break;
	case T_SYMBOL:
		outsymbol((char *)data);
		break;
	case T_LITERAL:
		outstr((const char *)data);
		break;
	case T_PTR:
		outstr("__ptr");
		break;
	case T_VALUE:
		outdec((int)data);
		break;
	case T_STACK:
		outstr("__stack");
		break;
	}
}

void dump_ins (INS *tmp)
{
	FILE *save = output;

	output = stdout;
	gen_code(tmp);
	output = save;
}

/*
 *	gen assembly code
 *
 */
void gen_code (INS *tmp)
{
	int code;
	int type;
	intptr_t data;
	int imm_type;
	intptr_t imm_data;

	code = tmp->code;
	type = tmp->type;
	data = tmp->data;
	imm_type = tmp->imm_type;
	imm_data = tmp->imm_data;

	if (type == T_NOP)
		return;

	switch (code) {
	case I_FARPTR:
		ot("__farptr\t");

		switch (type) {
		case T_LABEL:
			outlabel((int)data);
			break;
		case T_SYMBOL:
			outsymbol((char *)data);
			break;
		}
		outstr(",");
		outstr(tmp->arg[0]);
		outstr(",");
		outstr(tmp->arg[1]);
		nl();
		break;

	case I_FARPTR_I:
		ot("__farptr_i\t");
		outsymbol((char *)data);
		outstr(",");
		outstr(tmp->arg[0]);
		outstr(",");
		outstr(tmp->arg[1]);
		nl();
		break;

	case I_FARPTR_GET:
		ot("__farptr_get\t");
		outstr(tmp->arg[0]);
		outstr(",");
		outstr(tmp->arg[1]);
		nl();
		break;

	case I_FGETB:
		ot("__farptr_i\t");
		outsymbol((char *)data);
		nl();
		ol("__fgetb");
		break;

	case I_FGETUB:
		ot("__farptr_i\t");
		outsymbol((char *)data);
		nl();
		ol("__fgetub");
		break;

	case I_FGETW:
		ot("__farptr_i\t");
		outsymbol((char *)data);
		nl();
		ol("  jsr\t_farpeekw.fast");
		break;

	case I_LDB:
	case I_LDBY:
		if (code == I_LDBY)
			ot("__ldby\t");
		else
			ot("__ldb\t");
		out_type(type, data);
		nl();
		break;

	case I_LDYB:
		ot("  ldy\t");
		out_type(type, data);
		nl();
		break;

	case I_LDUB:
		ot("__ldub\t");
		out_type(type, data);
		nl();
		break;

	case I_LDBP:
		ot("__ldbp\t");
		out_addr(type, data);
		nl();
		break;

	case I_LDUBP:
		ot("__ldubp\t");
		out_addr(type, data);
		nl();
		break;

	case I_STBP:
		ot("__stbp\t");
		out_addr(type, data);
		nl();
		break;

	case I_LDW:
		ot("__ldw\t");
		out_addr(type, data);
		nl();
		break;

	case I_LDWI:
		ot("__ldwi\t");
		out_type(type, data);
		nl();
		break;

	case I_LDWP:
		ot("__ldwp\t");
		out_addr(type, data);
		nl();
		break;

	case I_STWP:
		ot("__stwp\t");
		out_addr(type, data);
		nl();
		break;

	case I_STB:
		ot("__stb\t");
		out_addr(type, data);
		nl();
		break;

	case I_STBPS:
		ol("__stbps");
		break;

	case I_STW:
		ot("__stw\t");
		if (type == T_PTR)
			outstr("<");
		out_addr(type, data);
		nl();
		break;

	case I_STWI:
	case I_STBI:
		if (code == I_STWI)
			ot("__stwi\t");
		else
			ot("__stbi\t");
		out_type(type, data);
		outstr(", ");
		out_type(imm_type, imm_data);
		nl();
		break;

	case I_STWZ:
		ot("__stwz\t");
		out_type(type, data);
		nl();
		break;

	case I_STBZ:
		ot("__stbz\t");
		out_type(type, data);
		nl();
		break;
	case I_STWPS:
		ol("__stwps");
		break;

	case I_ADDW:
		ot("__addw\t");
		out_addr(type, data);
		nl();
		break;

	case I_ADDB:
		ot("__addb\t");
		out_addr(type, data);
		nl();
		break;

	case I_ADDUB:
		ot("__addub\t");
		out_addr(type, data);
		nl();
		break;

	case I_ADDWI:
	case I_ADDBI:
		if (code == I_ADDBI)
			ot("__addbi");
		else
			ot("__addwi");
		/* Assembler workaround; pceas doesn't like if the code
		   size changes as it resolved a symbol, so we use the
		   variant without ".if"s if there is a symbol involved. */
		if (type == T_SYMBOL ||
		    type == T_LITERAL ||
		    type == T_STRING)
			outstr("_sym");
		outstr("\t");
		out_type(type, data);
		nl();
		break;

	case I_ADDBI_P:
		ot("__addbi_p\t");
		out_type(type, data);
		nl();
		break;

	case I_ADDWS:
		ol("__addws");
		break;

	case I_ADDBS:
		ol("__addbs");
		break;

	case I_MODSP:
		ot("__modsp");
		if (type == T_LITERAL) {
			outstr("_sym\t");
			outstr((const char *)data);
		}
		else {
			outstr("\t");
			outdec((int)data);
		}
		nl();
		break;

	case I_SUBW:
		ot("__subw\t");
		out_addr(type, data);
		nl();
		break;

	case I_SUBWI:
		ot("__subwi\t");
		outdec((int)data);
		nl();
		break;

	case I_SUBWS:
		ol("__subws");
		break;

	case I_ORWI:
		ot("__orwi\t");
		outdec((int)data);
		nl();
		break;

	case I_STBIP:
		ot("__stbip\t");
		outdec((int)data);
		nl();
		break;

	case I_STWIP:
		ot("__stwip\t");
		outdec((int)data);
		nl();
		break;

	case I_ORWS:
		ol("__orws");
		break;

	case I_EORWS:
		ol("__eorws");
		break;

	case I_ANDWI:
		ot("__andwi\t");
		outdec((int)data);
		nl();
		break;

	case I_ANDWS:
		ol("__andws");
		break;

	case I_ASLW:
		ol("__aslw");
		break;

	case I_ASLWI:
		ot("__aslwi\t");
		out_type(type, data);
		nl();
		break;

	case I_LSRWI:
		ot("__lsrwi\t");
		out_type(type, data);
		nl();
		break;

	case I_ASRWI:
		ot("__asrwi\t");
		out_type(type, data);
		nl();
		break;

	case I_ASLWS:
		ol("__aslws");
		break;

	case I_ASRW:
		ol("__asrw");
		break;

	case I_COMW:
		ol("__comw");
		break;

	case I_NEGW:
		ol("__negw");
		break;

	case I_EXTW:
		ol("__extw");
		break;

	case I_BOOLW:
		ol("__boolw");
		break;

	case I_NOTW:
		ol("__notw");
		break;

	case I_JMP:
		ot("  jmp\t");
		outsymbol((char *)data);
		nl();
		break;

	case I_JSR:
		ot("  jsr\t");

		switch (type) {
		case T_SYMBOL:
			outsymbol((char *)data);
			break;
		case T_LIB:
			outstr((const char *)data);
			break;
		}
		nl();
		break;

	case I_CALL:
		switch (type) {
		case T_SYMBOL:
			ot("  call\t");
			outsymbol((char *)data);
			if (imm_data) {
				outstr(".");
				outdec((int)imm_data);
			}
			break;
		case T_LIB:
			ot("  jsr\t");
			outstr((const char *)data);
			break;
		}
		nl();
		break;

	case I_MACRO:
		switch (type) {
		case T_SYMBOL:
			ot("  \t");
			outsymbol((char *)data);
			if (imm_data) {
				outstr(".");
				outdec((int)imm_data);
			}
			break;
		case T_LIB:
			ot("  \t");
			outstr((const char *)data);
			break;
		}
		nl();
		break;

	case I_CALLP:
		ol("__callp");
		break;

	case I_ENTER:
		ot("__enter\t");
		outsymbol((char *)data);
		nl();
		break;

	case I_LEAVE:
		ot("__leave\t");
		outdec((int)data);
		nl();
		break;

	case I_PUSHW:
		ol("__pushw");
		break;

	case I_POPW:
		ol("__popw");
		break;

	case I_SPUSHW:
		ol("__spushw");
		break;

	case I_SPOPW:
		ol("__spopw");
		break;

	case I_SPUSHB:
		ol("__spushb");
		break;

	case I_SPOPB:
		ol("__spopb");
		break;

	case I_TSTW:
		ol("__tstw");
		break;

	case I_BRA:
		ot("__bra\t");
		outlabel((int)data);
		nl();
		break;

	case I_BEQ:
		ot("__beq\t");
		outlabel((int)data);
		nl();
		break;

	case I_BNE:
		ot("__bne\t");
		outlabel((int)data);
		nl();
		break;

	case I_CMPWI_EQ:
		ot("__cmpwi_eq\t");
		out_type(type, data);
		nl();
		break;

	case I_CMPWI_NE:
		ot("__cmpwi_ne\t");
		out_type(type, data);
		nl();
		break;

	case I_BANK:
		ot(".bank\t");
		switch (type) {
		case T_VALUE:
			outdec((int)data);
			break;
		default:
			error("bank type switching not handled");
			break;
		}
		nl();
		break;

	case I_OFFSET:
		ot(".org\t");
		switch (type) {
		case T_VALUE:
			outhex((unsigned short)data);
			break;
		default:
			error("org type setting not handled");
			break;
		}
		nl();
		break;

	case I_INCW:
		ot("incw\t");
		out_addr(type, data);
		nl();
		break;

	case I_INCB:
		ot("inc\t");
		out_addr(type, data);
		nl();
		break;

	case I_LABEL:
		outlabel((int)data);
		col();
		break;

	case I_MULWI:
		ot("__mulwi\t");
		outdec((int)data);
		nl();
		break;

	case I_EXTUW:
		ol("__extuw");
		break;

	case I_DEF:
		outstr((const char *)data);
		outstr(" .equ ");
		outdec((int)imm_data);
		nl();
		break;

	case I_SEI:
		ol("sei");
		break;

	case I_CLI:
		ol("cli");
		break;

	case I_SAVESP:
		ol("__savesp");
		break;

	case I_LOADSP:
		ol("__loadsp");
		break;

	case I_GETACC:
		ol("__getacc");
		break;

	default:
		gen_asm(tmp);
		break;
	}
}

/* ----
 * gen_asm()
 * ----
 * generate optimizer asm code
 *
 */
void gen_asm (INS *inst)
{
	switch (inst->code) {
	case X_LDB_P:
		ol("__ldb_p");
		break;

	case X_LDUB_P:
		ol("__ldub_p");
		break;

	case X_LDB_S:
		ot("__ldb_s\t");
		outdec((int)inst->data);
		nl();
		break;

	case X_LDUB_S:
		ot("__ldub_s\t");
		outdec((int)inst->data);
		nl();
		break;

	case X_LDW_S:
		ot("__ldw_s\t");
		outdec((int)inst->data);
		nl();
		break;

	case X_LDD_I:
		ot("__ldd_i\t");
		outdec((int)inst->data);
		outstr(",");
		outsymbol(inst->arg[0]);
		outstr(",");
		outsymbol(inst->arg[1]);
		nl();
		break;

	case X_LDD_B:
		ot("__ldd_b\t");
		outsymbol((char *)inst->data);
		outstr(",");
		outsymbol(inst->arg[0]);
		outstr(",");
		outsymbol(inst->arg[1]);
		nl();
		break;

	case X_LDD_W:
		ot("__ldd_w\t");
		outsymbol((char *)inst->data);
		outstr(",");
		outsymbol(inst->arg[0]);
		outstr(",");
		outsymbol(inst->arg[1]);
		nl();
		break;

	case X_LDD_S_B:
		ot("__ldd_s_b\t");
		outdec((int)inst->data);
		outstr(",");
		outsymbol(inst->arg[0]);
		outstr(",");
		outsymbol(inst->arg[1]);
		nl();
		break;

	case X_LDD_S_W:
		ot("__ldd_s_w\t");
		outdec((int)inst->data);
		outstr(",");
		outsymbol(inst->arg[0]);
		outstr(",");
		outsymbol(inst->arg[1]);
		nl();
		break;

	case X_LEA_S:
		ot("__lea_s\t");
		outdec((int)inst->data);
		nl();
		break;

	case X_PEA_S:
		ot("__pea_s\t");
		outdec((int)inst->data);
		nl();
		break;

	case X_STBI_S:
		ot("__stbi_s\t");
		outdec((int)inst->imm_data);
		outstr(",");
		outdec((int)inst->data);
		nl();
		break;

	case X_STWI_S:
		ot("__stwi_s\t");
		outdec((int)inst->imm_data);
		outstr(",");
		outdec((int)inst->data);
		nl();
		break;

	case X_STW_S:
		ot("__stw_s\t");
		outdec((int)inst->data);
		nl();
		break;

	case X_STB_S:
		ot("__stb_s\t");
		outdec((int)inst->data);
		nl();
		break;

	case X_INCW_S:
		ot("__incw_s\t");
		outdec((int)inst->data);
		nl();
		break;

	case X_INCB_S:
		ot("__incb_s\t");
		outdec((int)inst->data);
		nl();
		break;

	case X_ADDW_S:
		ot("__addw_s\t");
		outdec((int)inst->data);
		nl();
		break;
	case X_ADDB_S:
		ot("__addb_s\t");
		outdec((int)inst->data);
		nl();
		break;
	case X_ADDUB_S:
		ot("__addub_s\t");
		outdec((int)inst->data);
		nl();
		break;
	default:
		error("internal error: invalid instruction");
		break;
	}
}
