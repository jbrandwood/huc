/*	File gen.c: 2.1 (83/03/20,16:02:06) */
/*% cc -O -c %
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include "defs.h"
#include "data.h"
#include "code.h"
#include "primary.h"
#include "sym.h"
#include "gen.h"
#include "expr.h"
#include "const.h"
#include "error.h"
#include "io.h"

/*
 *	return next available internal label number
 *
 */
int getlabel (void)
{
	return (nxtlab++);
}

/*
 *	fetch a static memory cell into the primary register
 */
void getmem (SYMBOL *sym)
{
	char *data;

	if ((sym->identity != POINTER) && (sym->sym_type == CCHAR || sym->sym_type == CUCHAR)) {
		int op = (sym->sym_type & CUNSIGNED) ? I_LD_UM : I_LD_BM;
		if ((sym->storage & STORAGE) == LSTATIC)
			out_ins(op, T_SYMBOL, (intptr_t)(sym->linked));
		else
			out_ins(op, T_SYMBOL, (intptr_t)sym);
	}
	else {
		if ((sym->storage & STORAGE) == LSTATIC)
			out_ins(I_LD_WM, T_SYMBOL, (intptr_t)(sym->linked));
		else if ((sym->storage & STORAGE) == CONST && (data = get_const(sym)))
			out_ins(I_LD_WI, T_LITERAL, (intptr_t)data);
		else
			out_ins(I_LD_WM, T_SYMBOL, (intptr_t)sym);
	}
}

/*
 *	fetch the address of the specified symbol into the primary register
 *
 */
void getloc (SYMBOL *sym)
{
	int value;

	if ((sym->storage & STORAGE) == LSTATIC)
		out_ins(I_LD_WI, T_SYMBOL, (intptr_t)(sym->linked));
	else {
#if ULI_NORECURSE
		value = sym->offset;
		if (norecurse && value < 0) {
			/* XXX: bit of a memory leak, but whatever... */
			SYMBOL * locsym = copysym(sym);
			if (NAMEALLOC <=
				sprintf(locsym->name, "_%s_end - %d", current_fn, -value))
				error("norecurse local name too long");
			locsym->linked = sym;
			out_ins(I_LD_WI, T_SYMBOL, (intptr_t)locsym);
		}
		else {
			value -= stkp;
			out_ins_sym(I_LEA_S, T_STACK, value, sym);
		}
#else
		value = sym->offset - stkp;
		out_ins_sym(I_LEA_S, T_STACK, value, sym);
#endif
	}
}

/*
 *	store the primary register into the specified static memory cell
 *
 */
void putmem (SYMBOL *sym)
{
	int code;

	/* XXX: What about 1-byte structs? */
	if ((sym->identity != POINTER) & (sym->sym_type == CCHAR || sym->sym_type == CUCHAR))
		code = I_ST_UM;
	else
		code = I_ST_WM;

	if ((sym->storage & STORAGE) == LSTATIC)
		out_ins(code, T_SYMBOL, (intptr_t)(sym->linked));
	else
		out_ins(code, T_SYMBOL, (intptr_t)sym);
}

/*
 *	store the specified object type in the primary register
 *	at the address on the top of the stack
 *
 */
void putstk (char typeobj)
{
	if (typeobj == CCHAR || typeobj == CUCHAR)
		out_ins(I_ST_UT, 0, 0);
	else
		out_ins(I_ST_WT, 0, 0);
}

/*
 *	fetch the specified object type indirect through the primary
 *	register into the primary register
 *
 */
void indirect (char typeobj)
{
	out_ins(I_ST_WM, T_PTR, 0);
	if (typeobj == CCHAR)
		out_ins(I_LD_BP, T_PTR, 0);
	else
	if (typeobj == CUCHAR)
		out_ins(I_LD_UP, T_PTR, 0);
	else
		out_ins(I_LD_WP, T_PTR, 0);
}

void farpeek (SYMBOL *ptr)
{
	if (ptr->sym_type == CCHAR)
		out_ins(I_FGETB, T_SYMBOL, (intptr_t)ptr);
	else if (ptr->sym_type == CUCHAR)
		out_ins(I_FGETUB, T_SYMBOL, (intptr_t)ptr);
	else
		out_ins(I_FGETW, T_SYMBOL, (intptr_t)ptr);
}

/*
 *	print partial instruction to get an immediate value into
 *	the primary register
 *
 */
void immed (int type, intptr_t data)
{
	if (type == T_VALUE && (data < -32768 || data > 65535))
		warning(W_GENERAL, "large integer truncated");
	if (type == T_SYMBOL) {
		SYMBOL *sym = (SYMBOL *)data;
		if ((sym->storage & STORAGE) == LSTATIC)
			out_ins(I_LD_WI, T_SYMBOL, (intptr_t)(sym->linked));
		else
			out_ins(I_LD_WI, T_SYMBOL, data);
	} else {
		out_ins(I_LD_WI, type, data);
	}
}

/*
 *	push the primary register onto the stack
 *
 */
void gpush (void)
{
	out_ins(I_PUSH_WR, T_VALUE, INTSIZE);
}

/*
 *	push the primary register onto the argument stack
 *
 */
void gpusharg (int size)
{
	out_ins(I_PUSHARG_WR, T_SIZE, size);
	stkp = stkp - size;
}

/*
 *	pop the top of the stack into the secondary register
 *
 */
void gpop (void)
{
	out_ins(I_POP_WR, 0, 0);
}

/*
 *	call the specified subroutine name
 *
 */
void gcall (char *sname, int nargs)
{
	out_ins_ex(I_CALL, T_LITERAL, (intptr_t)sname, T_VALUE, nargs);
}

/*
 *	call the specified macro name
 *
 */
void gmacro (char *sname, int nargs)
{
	out_ins_ex(I_MACRO, T_LITERAL, (intptr_t)sname, T_VALUE, nargs);
}

/*
 *	jump to specified internal label number
 *
 */
void jump (int label)
{
	out_ins(I_BRA, T_LABEL, label);
}

/*
 *	test the primary register and jump if false to label
 *
 */
void testjump (int label, int ft)
{
	out_ins(I_TST_WR, 0, 0);
	if (ft)
		out_ins(I_BTRUE, T_LABEL, label);
	else
		out_ins(I_BFALSE, T_LABEL, label);
}

/*
 *	modify the stack pointer to the new value indicated
 *      Is it there that we decrease the value of the stack to add local vars ?
 */
int modstk (int newstkp)
{
	int k;

//	k = galign(newstkp - stkp);
	k = newstkp - stkp;
	if (k) {
		gtext();
		out_ins(I_MODSP, T_STACK, k);
	}
	return (newstkp);
}

/*
 *	multiply the primary register by INTSIZE
 */
void gaslint (void)
{
	out_ins(I_ASL_WR, 0, 0);
}

/*
 *	divide the primary register by INTSIZE
 */
void gasrint (void)
{
	out_ins(I_ASR_WI, T_VALUE, 1);
}

/*
 *	mark the start of a case statement
 */
void gcase (int nlab, int value)
{
	out_ins(I_ENDCASE, 0, 0);
	gnlabel(nlab);
	out_ins(I_CASE, T_VALUE, value);
}

/*
 *	mark the start of a default statement
 */
void gdefault (int nlab)
{
	out_ins(I_ENDCASE, 0, 0);
	gnlabel(nlab);
	out_ins(I_DEFAULT, 0, 0);
}

/*
 *	multiply the primary and secondary registers
 *	(result in primary)
 *
 */
void gmult (int is_unsigned)
{
	/* the bottom 16-bits of a signed and unsigned 16-bit multiply are identical! */
	if (is_unsigned)
		out_ins(I_MUL_WT, 0, 0);
	else
		out_ins(I_MUL_WT, 0, 0);
}

void gmult_imm (int value)
{
	out_ins(I_MUL_WI, T_VALUE, (intptr_t)value);
}

/*
 *	divide the secondary register by the primary
 *	(quotient in primary, remainder in secondary)
 *
 */
void gdiv (int is_unsigned)
{
	if (is_unsigned)
		out_ins(I_UDIV_WT, 0, 0);
	else
		out_ins(I_SDIV_WT, 0, 0);
}

void gdiv_imm (int value)
{
	gpush();
	immed(T_VALUE, value);
	gdiv(1);
}

/*
 *	compute the remainder (mod) of the secondary register
 *	divided by the primary register
 *	(remainder in primary, quotient in secondary)
 *
 */
void gmod (int is_unsigned)
{
	if (is_unsigned)
		out_ins(I_UMOD_WT, 0, 0);
	else
		out_ins(I_SMOD_WT, 0, 0);
}

/*
 *	arithmetic shift right the secondary register the number of
 *	times in the primary register
 *	(results in primary register)
 *
 */
void gasr (int is_unsigned)
{
	if (is_unsigned)
		out_ins(I_LSR_WT, 0, 0);
	else
		out_ins(I_ASR_WT, 0, 0);
}

/*
 *	arithmetic shift left the secondary register the number of
 *	times in the primary register
 *	(results in primary register)
 *
 */
void gasl (void)
{
	out_ins(I_ASL_WT, 0, 0);
}

/*
 *	two's complement of primary register
 *
 */
void gneg (void)
{
	out_ins(I_NEG_WR, 0, 0);
}

/*
 *	one's complement of primary register
 *
 */
void gcom (void)
{
	out_ins(I_COM_WR, 0, 0);
}

/*
 *	convert primary register into logical value
 *
 */
void gbool (void)
{
	out_ins(I_BOOLEAN, 0, 0);
}

/*
 *	boolean logical test of primary register
 *
 */
void gtest (void)
{
	out_ins(I_TST_WR, 0, 0);
}

/*
 *	boolean logical complement of primary register
 *
 */
void gnot (void)
{
	out_ins(I_NOT_WR, 0, 0);
	out_ins(I_BOOLEAN, 0, 0);
}

/*
 *	increment the primary register by 1 if char, INTSIZE if int
 */
void ginc (LVALUE *lval)
{
	SYMBOL *sym = lval->symbol;

	if (lval->ptr_type == CINT || lval->ptr_type == CUINT ||
	    (sym && (sym->ptr_order > 1 || (sym->identity == ARRAY && sym->ptr_order > 0))))
		out_ins(I_ADD_WI, T_VALUE, 2);
	else if (lval->ptr_type == CSTRUCT)
		out_ins(I_ADD_WI, T_VALUE, lval->tagsym->size);
	else
		out_ins(I_ADD_WI, T_VALUE, 1);
}

/*
 *	decrement the primary register by one if char, INTSIZE if int
 */
void gdec (LVALUE *lval)
{
	SYMBOL *sym = lval->symbol;

	if (lval->ptr_type == CINT || lval->ptr_type == CUINT ||
	    (sym && (sym->ptr_order > 1 || (sym->identity == ARRAY && sym->ptr_order > 0))))
		out_ins(I_SUB_WI, T_VALUE, 2);
	else if (lval->ptr_type == CSTRUCT)
		out_ins(I_SUB_WI, T_VALUE, lval->tagsym->size);
	else
		out_ins(I_SUB_WI, T_VALUE, 1);
}

void scale_const (int type, int otag, int *size)
{
	switch (type) {
	case CINT:
	case CUINT:
		*size += *size;
		break;
	case CSTRUCT:
		*size *= tag_table[otag].size;
		break;
	default:
		break;
	}
}

void gcast (int type)
{
	switch (type) {
	case CCHAR:
		out_ins(I_EXT_BR, 0, 0);
		break;
	case CUCHAR:
		out_ins(I_EXT_UR, 0, 0);
		break;
	case CINT:
	case CUINT:
	case CVOID:
		break;
	default:
		abort();
	}
	;
}

void gsei (void)
{
	out_ins(I_SEI, 0, 0);
}
void gcli (void)
{
	out_ins(I_CLI, 0, 0);
}
void gfence (void)
{
	out_ins(I_FENCE, 0, 0);
}
void gshort (void)
{
	out_ins(I_SHORT, 0, 0);
}
