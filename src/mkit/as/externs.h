extern unsigned char rom[MAX_BANKS][8192];
extern unsigned char map[MAX_BANKS][8192];
extern uint32_t dbg_info[MAX_BANKS][8192];
extern uint8_t dbg_column[MAX_BANKS][8192];

extern char bank_name[MAX_BANKS][64];
extern int bank_loccnt[MAX_S][MAX_BANKS];
extern int bank_page[MAX_S][MAX_BANKS];
extern int bank_maxloc[MAX_BANKS];              /* record max location in bank */

extern unsigned char workspace[65536];          /* buffer for .inc and .def directives */

extern int discontiguous;                       /* NZ signals a warp in loccnt */
extern int max_zp;                              /* higher used address in zero page */
extern int max_bss;                             /* higher used address in ram */
extern int max_bank;                            /* last bank used */
extern int data_loccnt;                         /* data location counter */
extern int data_size;                           /* size of binary output (in bytes) */
extern int data_level;                          /* data output level, must be <= listlevel to be outputed */
extern int phase_offset;                        /* location counter offset for .phase */
extern int phase_bank;                          /* location counter bank for .phase */
extern int loccnt;                              /* location counter */
extern int bank;                                /* current bank */
extern int bank_base;                           /* bank base index */
extern int bank_limit;                          /* bank limit */
extern int rom_limit;                           /* rom max. size in bytes */
extern int page;                                /* page */
extern int rs_base;                             /* .rs counter */
extern int rs_mprbank;                          /* .rs counter */
extern int rs_overlay;                          /* .rs counter */
extern unsigned char section;                   /* current section: S_ZP, S_BSS, S_CODE or S_DATA */
extern int section_bank[MAX_S];                 /* current bank for each section */
extern int section_phase[MAX_S];                /* current phase offset for each section */
extern int section_flags[MAX_S];                /* current flags for each section */
extern int section_limit[MAX_S];                /* current loccnt limit for each section */
extern int xdata_bank;                          /* initial bank for .XDATA, relocated before LAST_PASS */
extern int xdata_addr;                          /* initial addr for .XDATA, relocated before LAST_PASS */
extern int xinit_bank;                          /* initial bank for .XINIT, relocated before LAST_PASS */
extern int xinit_addr;                          /* initial addr for .XINIT, relocated before LAST_PASS */
extern int xstrz_bank;                          /* initial bank for .XSTRZ, relocated before LAST_PASS */
extern int xstrz_addr;                          /* initial addr for .XSTRZ, relocated before LAST_PASS */
extern int in_if;                               /* true if in a '.if' statement */
extern int if_expr;                             /* set when parsing an .if expression */
extern int if_level;                            /* level of nested .if's */
extern int if_line[256];                        /* .if line number */
extern int skip_lines;                          /* when true skip lines */
extern int continued_line;                      /* set when a line is the continuation of another line */
extern int pcx_w, pcx_h;                        /* PCX dimensions */
extern int pcx_nb_colors;                       /* number of colors (16/256) in the PCX */
extern int pcx_nb_args;                         /* number of argument */
extern unsigned int pcx_arg[8];                 /* PCX args array */
extern t_symbol *pcx_lbl[8];                    /* PCX args array (labels) */
extern unsigned char *pcx_buf;                  /* pointer to the PCX buffer */
extern unsigned char pcx_pal[256][3];           /* palette */
extern unsigned int tile_offset;                /* offset in the tile reference table */
extern struct t_tile tile[65536 / 32];          /* tile info table */
extern struct t_tile *tile_tbl[HASH_COUNT];     /* tile hash table */
extern struct t_symbol *tile_lablptr;           /* tile symbol reference */
extern struct t_symbol *blk_lablptr;            /* meta-tile symbol reference */
extern char *expr;                              /* expression string pointer */
extern struct t_symbol *expr_toplabl;           /* pointer to the innermost scope-label */
extern struct t_symbol *expr_lablptr;           /* pointer to the last-referenced label */
extern int expr_lablcnt;                        /* number of label seen in an expression */
extern int expr_mprbank;                        /* last-defined bank# in an expression */
extern int expr_overlay;                        /* last-defined overlay# in an expression */
extern int complex_expr;                        /* NZ if an expression contains operators */
extern int mopt;
extern int in_macro;
extern int expand_macro;
extern char marg[8][10][256];
extern int midx;
extern int mcounter, mcntmax;
extern int mcntstack[8];
extern t_line *mstack[8];
extern t_line *mlptr;
extern t_macro *macro_tbl[HASH_COUNT];
extern t_macro *mptr;
extern t_func *func_tbl[HASH_COUNT];
extern t_func *func_ptr;
extern t_proc *proc_ptr;
extern int proc_nb;
extern char func_arg[8][10][80];
extern int func_idx;
extern int infile_error;
extern int infile_num;
extern FILE *out_fp;                            /* file pointers, output */
extern FILE *in_fp;                             /* input */
extern FILE *lst_fp;                            /* listing */
extern int lst_line;                            /* listing */
extern t_file *lst_tfile;                       /* listing */
extern int debug_line;                          /* .DBG info */
extern int debug_column;                        /* .DBG info */
extern t_file *debug_file;                      /* .DBG info */
extern char *section_name[MAX_S + 1];
extern t_file *extra_file;
extern t_input input_file[MAX_NESTING + 1];
extern char full_path[PATHSZ * 2];              /* full path name of last file opened */

extern t_machine *machine;
extern t_machine nes;
extern t_machine pce;
extern t_machine fuji;
extern t_opcode *inst_tbl[HASH_COUNT];          /* instructions hash table */
extern t_symbol *hash_tbl[HASH_COUNT];          /* label hash table */
extern t_symbol *lablptr;                       /* label pointer into symbol table */
extern t_symbol *glablptr;                      /* pointer to the latest defined global symbol */
extern t_symbol *scopeptr;                      /* pointer to the latest defined scope label */
extern t_symbol *lastlabl;                      /* last label we have seen */
extern t_symbol *bank_glabl[MAX_S][MAX_BANKS];  /* latest global label in each bank */
extern t_symbol *unaliased;                     /* unaliased version of last symbol lookup */
extern t_branch *branchlst;                     /* first branch instruction assembled */
extern t_branch *branchptr;                     /* last branch instruction assembled */

extern int branches_changed;                    /* count of branches changed in pass */
extern char need_another_pass;                  /* NZ if another pass if required */
extern char hex[];                              /* hexadecimal character buffer */
extern int stop_pass;                           /* stop the program; set by fatal_error() */
extern int errcnt;                              /* error counter */
extern int no_rom_file;                         /* NZ if only assembling data file(s) */
extern void (*opproc)(int *);                   /* instruction gen proc */
extern int opflg;                               /* instruction flags */
extern int opval;                               /* instruction value */
extern int optype;                              /* instruction type */
extern char opext;                              /* instruction extension (.l or .h) */
extern int pass;                                /* pass type (FIRST_PASS, EXTRA_PASS, LAST_PASS */
extern int pass_count;                          /* pass counter */
extern char prlnbuf[];                          /* input line buffer */
extern char tmplnbuf[];                         /* temporary line buffer */
extern int slnum;                               /* source line number counter */
extern char symbol[];                           /* temporary symbol storage */
extern int undef;                               /* undefined symbol in expression flag */
extern int notyetdef;                           /* undefined-in-current-pass symbol in expr */
extern unsigned int value;                      /* operand field value */
extern int newproc_opt;                         /* use "new" style of procedure thunks */
extern int strip_opt;                           /* strip unused procedures? */
extern int kickc_opt;                           /* NZ if -kc flag on command line */
extern int hucc_opt;                            /* NZ if -hucc flag on command line */
extern int mlist_opt;                           /* macro listing main flag */
extern int xlist;                               /* listing file main flag */
extern int debug_format;                        /* debug output type */
extern int list_level;                          /* output level */
extern int asm_opt[MAX_OPTS];                   /* assembler option state */
extern int opvaltab[6][16];
extern int call_bank;                           /* bank for .proc thunks */
extern int kickc_mode;                          /* NZ if currently in KickC mode */
extern int sdcc_mode;                           /* NZ if assembling SDCC code */
extern int hucc_mode;                           /* NZ if assembling HuCC code */
extern int kickc_final;                         /* auto-include "kickc-final.asm" */
extern int hucc_final;                          /* auto-include "hucc-final.asm" */
extern int in_final;                            /* set when in xxxx-final.asm include */
extern int preproc_inblock;                     /* C-style comment: within block comment */
extern int preproc_sfield;                      /* C-style comment: SFIELD as a variable */
extern int preproc_modidx;                      /* C-style comment: offset to modified char */

/* this is set when suppressing the listing output of stripped procedures */
/* n.b. fully compatible with 2-pass assembly because code is still built */
extern int cloaking_stripped;

/* this is set when not assembling the code within the stripped procedure */
/* n.b. not compatible with 2-pass assembly because symbol addresses will */
/* change because both multi-label and branch tracking counts will change */
extern int skipping_stripped;

/* this is set to say that skipping is an acceptable alternative to */
/* cloaking, which means that we've decided to do a 3-pass assembly */
extern int allow_skipping;
