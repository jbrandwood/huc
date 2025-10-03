/*	File pragma.c: 2.1 (00/08/09,04:59:24) */
/*% cc -O -c %
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "data.h"
#include "error.h"
#include "io.h"
#include "lex.h"
#include "pragma.h"
#include "sym.h"
#include "fastcall.h"

/* locals */
struct fastcall ftemp;
struct fastcall *fastcall_tbl[256];
static char cmd[LINESIZE];
static const char *cmdptr;

/* protos */
int fastcall_look (const char *fname, int nargs, struct fastcall **p);


/* ----
 * dopragma()
 * ----
 * handle pragma directive
 *
 */
void dopragma (void)
{
	int i;

	/* make a local copy of the pragma command line */
	for (i = 0;; i++) {
		if (ch() == 0)
			break;
		cmd[i] = gch();
	}
	cmd[i] = 0;

	/* parse */
	parse_pragma();
}


/* ----
 * parse_pragma()
 * ----
 * parse pragma command line
 *
 */
void parse_pragma (void)
{
	char sname[NAMESIZE];

	/* get command */
	cmdptr = cmd;

	if (!symget(sname)) {
		error("illegal symbol name");
		return;
	}

	/* fastcall */
	if (strcmp(sname, "fastcall") == 0)
		new_fastcall();
/*		new_fastcall(sname); */
	else if (!strcmp(sname, "no_recursive"))
		norecurse = 1;
	else if (!strcmp(sname, "recursive"))
		norecurse = 0;
	/* others */
	else
		error("unknown pragma");
}


/* ----
 * add_fastcall()
 * ----
 * setup a new fastcall from a string
 *
 * ie. "func(word __dx, byte __al, byte __ah)"
 *
 */
void add_fastcall (const char *ident)
{
	cmdptr = ident;
	new_fastcall();
}


/* ----
 * new_fastcall()
 * ----
 * setup a new fastcall
 *
 * ie. #pragma fastcall func(word __dx, byte __al, byte __ah)
 *
 */
void new_fastcall (void)
{
	struct fastcall *ptr;
	char fname[NAMESIZE];
	char sname[NAMESIZE];
	int hash;
	int cnt;
	int i;

	ptr = &ftemp;
	cnt = 0;
	ptr->nargs = 0;
	ptr->flags = 0;

	/* get func name */
	if (!symget(fname)) {
		error("illegal symbol name");
		return;
	}

	/* open */
	if (!strmatch("(")) {
		error("missing bracket");
		return;
	}

	/* extract args (max. 8) */
	for (i = 0; i < 8; i++) {
		/* get type */
		if (!symget(sname)) {
			if (*cmdptr == ')')
				break;
			error("syntax error");
			return;
		}
		if (strcmp(sname, "byte") == 0)
			ptr->argtype[i] = TYPE_BYTE;
		else if (strcmp(sname, "word") == 0)
			ptr->argtype[i] = TYPE_WORD;
		else if (strcmp(sname, "farptr") == 0)
			ptr->argtype[i] = TYPE_FARPTR;
		else if (strcmp(sname, "dword") == 0)
			ptr->argtype[i] = TYPE_DWORD;
		else {
			error("fastcall unknown type");
			return;
		}

		/* get name */
		if (!symget(sname)) {
			/* auto */
			if (*cmdptr != ',')
				ptr->argtype[i] = TYPE_WORDACC;
			else {
				error("fastcall register missing");
				return;
			}
		}
		else {
			/* dword */
			if (ptr->argtype[i] == TYPE_DWORD) {
				/* ptr */
				if (strcmp(sname, "acc") == 0)
					strcpy(ptr->argname[i++], "#acc");
				else
					strcpy(ptr->argname[i++], sname);

				/* low word */
				if (*cmdptr++ != ':') {
					error("syntax error");
					return;
				}
				if (!symget(sname)) {
					error("illegal symbol name");
					return;
				}

				/* copy */
				strcpy(ptr->argname[i++], sname);

				/* high word */
				if (*cmdptr++ != '|') {
					error("syntax error");
					return;
				}
				if (!symget(sname)) {
					error("illegal symbol name");
					return;
				}

				/* copy */
				strcpy(ptr->argname[i], sname);
				cnt++;
			}

			/* far ptr */
			else if (ptr->argtype[i] == TYPE_FARPTR) {
				/* bank */
				strcpy(ptr->argname[i++], sname);
				ptr->argtype[i] = TYPE_WORD;

				/* addr */
				if (*cmdptr++ != ':') {
					error("syntax error");
					return;
				}
				if (!symget(sname)) {
					error("illegal symbol name");
					return;
				}

				/* copy */
				strcpy(ptr->argname[i], sname);
				cnt++;
			}

			/* other */
			else {
				if (strcmp(sname, "acc") == 0) {
					/* accumulator */
					ptr->argtype[i] = TYPE_WORDACC;
				}
				else {
					/* variable */
					strcpy(ptr->argname[i], sname);
					cnt++;
				}
			}
		}

		/* increment arg counter */
		ptr->nargs++;

		/* next */
		if (!strmatch(","))
			break;
	}

	/* close */
	if (!strmatch(")")) {
		error("missing bracket");
		return;
	}

	/* extra infos */
	if (cnt) {
		if (ptr->nargs > 1)
			ptr->flags |= FASTCALL_EXTRA;
	}
	if (symget(sname)) {
		if (strcmp(sname, "nop") == 0)
			ptr->flags |= FASTCALL_NOP;
		if (strcmp(sname, "macro") == 0)
			ptr->flags |= FASTCALL_MACRO;
	}

	/* copy func name */
	strcpy(ptr->fname, fname);

	/* search for multi-decl */
	if (fastcall_look(fname, ptr->nargs, NULL)) {
		error("already defined");
		return;
	}

	/* ok */
	ptr = (void *)malloc(sizeof(struct fastcall));

	if (ptr == NULL)
		error("out of memory");
	else {
		/* dup struct */
		*ptr = ftemp;

		/* add to hash table */
		hash = symhash(fname);
		ptr->next = fastcall_tbl[hash];
		fastcall_tbl[hash] = ptr;
	}
}


/* ----
 * fastcall_look()
 * ----
 * search a fastcall function
 *
 */
int fastcall_look (const char *fname, int nargs, struct fastcall **p)
{
	struct fastcall *ptr;
	struct fastcall *ref;
	int hash;
	int is_fc;

	/* search */
	hash = symhash(fname);
	ptr = fastcall_tbl[hash];
	ref = NULL;
	is_fc = 0;
	while (ptr) {
		if (strcmp(ptr->fname, fname) == 0) {
			is_fc++;
			if (nargs != -1) {
				if (ptr->nargs == nargs)
					ref = ptr;
			}
		}
		ptr = ptr->next;
	}
	if (nargs != -1) {
		if (!ref)
			is_fc = 0;
	}

	/* return result */
	if (p)
		*p = ref;
	return (is_fc);
}


/* ----
 * symhash()
 * ----
 * calculate the hash value of a symbol
 *
 */
int symhash (const char *sym)
{
	int i;
	char c;
	int hash = 0;

	/* calc hash value */
	for (i = 0;; i++) {
		c = sym[i];
		if (c == 0)
			break;
		hash += c;
		hash = (hash << 3) + (hash >> 5) + c;
	}

	/* ok */
	return (hash & 0xFF);
}


/* ----
 * symget()
 * ----
 * extract a symbol name
 *
 */
int symget (char *sname)
{
	int i;

	skip_blanks();

	/* first char must be alpha */
	if (!alpha(*cmdptr))
		return (0);

	/* extract symbol name (stops at first non-alphanum char) */
	for (i = 0;; i++) {
		if (!alphanum(*cmdptr))
			break;
		sname[i] = *cmdptr++;
	}
	sname[i] = 0;

	/* ok */
	return (1);
}


/* ----
 * strmatch()
 * ----
 * test if next input string is legal symbol name
 *
 */
int strmatch (char *lit)
{
	int i;

	skip_blanks();

	/* compare */
	i = streq(cmdptr, lit);

	if (i) {
		/* match */
		cmdptr += i;
		return (1);
	}

	/* different */
	return (0);
}


/* ----
 * skip_blanks()
 * ----
 * skips blank chars (stops at end of input line)
 *
 */
void skip_blanks (void)
{
	while ((*cmdptr == ' ') || (*cmdptr == '\t'))
		cmdptr++;
}
