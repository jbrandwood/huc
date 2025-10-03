#ifndef _INCLUDE_PREPROC_H
#define _INCLUDE_PREPROC_H

void doinclude (void);

void incl_globals_h (void);

FILE *open_include (void);

int init_path (void);

FILE *file_open (char *name, int initial_path);

void doasmdef (void);

void doasm (void);

void dodefine (void);

void doundef (void);

void preprocess (void);

void doifdef (bool ifdef);

int ifline (void);

void noiferr (void);

int cpp (int);

int keepch (char c);

void defmac (char *s);

void addmac (void);

void delmac (struct macro *mp);

struct macro *findmac (char *sname);

void toggle (char name, int onoff);

char **include_dirs (void);

#endif
