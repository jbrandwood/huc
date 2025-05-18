/* PCE.C */
void pce_write_header(FILE *f, int banks);
int  pce_scan_8x8_tile(unsigned int x, unsigned int y);
int  pce_scan_16x16_tile(unsigned int x, unsigned int y);
int  pce_pack_8x8_tile(unsigned char *buffer, void *data, int line_offset, int format);
int  pce_pack_16x16_tile(unsigned char *buffer, void *data, int line_offset, int format);
int  pce_pack_16x16_sprite(unsigned char *buffer, void *data, int line_offset, int format);
void pce_defchr(int *ip);
void pce_defpal(int *ip);
void pce_defspr(int *ip);
void pce_incbat(int *ip);
void pce_incpal(int *ip);
void pce_incspr(int *ip);
void pce_incmask(int *ip);
void pce_inctile(int *ip);
void pce_incblk(int *ip);
void pce_incmap(int *ip);
void pce_haltmap(int *ip);
void pce_maskmap(int *ip);
void pce_flagmap(int *ip);
void pce_vram(int *ip);
void pce_pal(int *ip);
void pce_develo(int *ip);
void pce_mml(int *ip);
void pce_incchrpal(int *ip);
void pce_incsprpal(int *ip);
void pce_inctilepal(int *ip);
void pce_outpng(int *ip);

/* MML.C */
int mml_start(unsigned char *buffer);
int mml_stop(unsigned char *buffer);
int mml_parse(unsigned char *buffer, int bufsize, char *ptr);

/* *INDENT-OFF* */
/* HuC6280 additional instructions (on top of Rockwell's r65C02) */
struct t_opcode huc6280_inst[38] = {
	{NULL, "BSR",  class2, 0, 0x44, 0},
	{NULL, "CLA",  class1, 0, 0x62, 0},
	{NULL, "CLX",  class1, 0, 0x82, 0},
	{NULL, "CLY",  class1, 0, 0xC2, 0},
	{NULL, "CSH",  class1, 0, 0xD4, 0},
	{NULL, "CSL",  class1, 0, 0x54, 0},
	{NULL, "SAX",  class1, 0, 0x22, 0},
	{NULL, "SAY",  class1, 0, 0x42, 0},
	{NULL, "SET",  class1, 0, 0xF4, 0},
	{NULL, "ST0",  class4, IMM, 0x03, 1},
	{NULL, "ST1",  class4, IMM, 0x13, 1},
	{NULL, "ST2",  class4, IMM, 0x23, 1},
	{NULL, "SXY",  class1, 0, 0x02, 0},
	{NULL, "TAI",  class6, 0, 0xF3, 0},
	{NULL, "TAM",  class8, 0, 0x53, 0},
	{NULL, "TAM0", class3, 0, 0x53, 0x01},
	{NULL, "TAM1", class3, 0, 0x53, 0x02},
	{NULL, "TAM2", class3, 0, 0x53, 0x04},
	{NULL, "TAM3", class3, 0, 0x53, 0x08},
	{NULL, "TAM4", class3, 0, 0x53, 0x10},
	{NULL, "TAM5", class3, 0, 0x53, 0x20},
	{NULL, "TAM6", class3, 0, 0x53, 0x40},
	{NULL, "TAM7", class3, 0, 0x53, 0x80},
	{NULL, "TDD",  class6, 0, 0xC3, 0},
	{NULL, "TIA",  class6, 0, 0xE3, 0},
	{NULL, "TII",  class6, 0, 0x73, 0},
	{NULL, "TIN",  class6, 0, 0xD3, 0},
	{NULL, "TMA",  class8, 0, 0x43, 0},
	{NULL, "TMA0", class3, 0, 0x43, 0x01},
	{NULL, "TMA1", class3, 0, 0x43, 0x02},
	{NULL, "TMA2", class3, 0, 0x43, 0x04},
	{NULL, "TMA3", class3, 0, 0x43, 0x08},
	{NULL, "TMA4", class3, 0, 0x43, 0x10},
	{NULL, "TMA5", class3, 0, 0x43, 0x20},
	{NULL, "TMA6", class3, 0, 0x43, 0x40},
	{NULL, "TMA7", class3, 0, 0x43, 0x80},
	{NULL, "TST",  class7, 0, 0x00, 0},
	{NULL, NULL, NULL, 0, 0, 0}
};

#define NARGS_0_1_2 0b11111000
#define NARGS_0_2_4 0b11101010
#define NARGS_1_3_5 0b11010101
#define NARGS_2_4_6 0b10101011
#define NARGS_0_1_2_3_5 0b11000000
#define NARGS_1_2_3_4_5_6 0b10000001

/* PCE specific pseudos */
struct t_opcode pce_pseudo[41] = {
	{NULL, "DEFCHR",     pce_defchr,    PSEUDO, P_DEFCHR,    0},
	{NULL, "DEFPAL",     pce_defpal,    PSEUDO, P_DEFPAL,    0},
	{NULL, "DEFSPR",     pce_defspr,    PSEUDO, P_DEFSPR,    0},
	{NULL, "INCBAT",     pce_incbat,    PSEUDO, P_INCBAT,    NARGS_1_2_3_4_5_6},
	{NULL, "INCSPR",     pce_incspr,    PSEUDO, P_INCSPR,    NARGS_0_1_2_3_5},
	{NULL, "INCPAL",     pce_incpal,    PSEUDO, P_INCPAL,    NARGS_0_1_2},
	{NULL, "INCTILE",    pce_inctile,   PSEUDO, P_INCTILE,   NARGS_0_2_4},
	{NULL, "INCBLK",     pce_incblk,    PSEUDO, P_INCBLK,    NARGS_2_4_6},
	{NULL, "INCMAP",     pce_incmap,    PSEUDO, P_INCMAP,    NARGS_1_3_5},
	{NULL, "INCCHRPAL",  pce_incchrpal, PSEUDO, P_INCCHRPAL, NARGS_0_2_4},
	{NULL, "INCSPRPAL",  pce_incsprpal, PSEUDO, P_INCSPRPAL, NARGS_0_2_4},
	{NULL, "INCTILEPAL", pce_inctilepal,PSEUDO, P_INCTILEPAL,NARGS_0_2_4},
	{NULL, "MML",        pce_mml,       PSEUDO, P_MML,       0},
	{NULL, "PAL",        pce_pal,       PSEUDO, P_PAL,       0},
	{NULL, "VRAM",       pce_vram,      PSEUDO, P_VRAM,      0},
	{NULL, "OUTPNG",     pce_outpng,    PSEUDO, P_OUTPNG,    0},
	{NULL, "INCMASK",    pce_incmask,   PSEUDO, P_INCMASK,   NARGS_0_1_2_3_5},
	{NULL, "HALTMAP",    pce_haltmap,   PSEUDO, P_HALTMAP,   NARGS_1_3_5},
	{NULL, "MASKMAP",    pce_maskmap,   PSEUDO, P_MASKMAP,   NARGS_2_4_6},
	{NULL, "FLAGMAP",    pce_flagmap,   PSEUDO, P_FLAGMAP,   NARGS_2_4_6},

	{NULL, ".DEFCHR",    pce_defchr,    PSEUDO, P_DEFCHR,    0},
	{NULL, ".DEFPAL",    pce_defpal,    PSEUDO, P_DEFPAL,    0},
	{NULL, ".DEFSPR",    pce_defspr,    PSEUDO, P_DEFSPR,    0},
	{NULL, ".INCBAT",    pce_incbat,    PSEUDO, P_INCBAT,    NARGS_1_2_3_4_5_6},
	{NULL, ".INCSPR",    pce_incspr,    PSEUDO, P_INCSPR,    NARGS_0_2_4},
	{NULL, ".INCPAL",    pce_incpal,    PSEUDO, P_INCPAL,    NARGS_0_1_2},
	{NULL, ".INCTILE",   pce_inctile,   PSEUDO, P_INCTILE,   NARGS_0_2_4},
	{NULL, ".INCBLK",    pce_incblk,    PSEUDO, P_INCBLK,    NARGS_2_4_6},
	{NULL, ".INCMAP",    pce_incmap,    PSEUDO, P_INCMAP,    NARGS_1_3_5},
	{NULL, ".INCCHRPAL", pce_incchrpal, PSEUDO, P_INCCHRPAL, NARGS_0_2_4},
	{NULL, ".INCSPRPAL", pce_incsprpal, PSEUDO, P_INCSPRPAL, NARGS_0_2_4},
	{NULL, ".INCTILEPAL",pce_inctilepal,PSEUDO, P_INCTILEPAL,NARGS_0_2_4},
	{NULL, ".MML",       pce_mml,       PSEUDO, P_MML,       0},
	{NULL, ".PAL",       pce_pal,       PSEUDO, P_PAL,       0},
	{NULL, ".VRAM",      pce_vram,      PSEUDO, P_VRAM,      0},
	{NULL, ".OUTPNG",    pce_outpng,    PSEUDO, P_OUTPNG,    0},
	{NULL, ".INCMASK",   pce_incmask,   PSEUDO, P_INCMASK,   NARGS_0_1_2_3_5},
	{NULL, ".HALTMAP",   pce_haltmap,   PSEUDO, P_HALTMAP,   NARGS_1_3_5},
	{NULL, ".MASKMAP",   pce_maskmap,   PSEUDO, P_MASKMAP,   NARGS_2_4_6},
	{NULL, ".FLAGMAP",   pce_flagmap,   PSEUDO, P_FLAGMAP,   NARGS_2_4_6},
	{NULL, NULL, NULL, 0, 0, 0}
};
/* *INDENT-ON* */

const char defdirs_pce[] =
#ifdef _WIN32
	"c:\\huc\\include\\huc"
#else
	"/usr/local/lib/huc/include/huc;" \
	"/usr/local/huc/include/huc;" \
	"/usr/local/share/huc/include/huc;" \
	"/usr/local/include/huc;" \
	"/usr/lib/huc/include/huc;" \
	"/usr/share/huc/include/huc;" \
	"/usr/include/huc"
#endif
;

extern struct t_opcode r65c02_inst[];

/* PCE machine description */
struct t_machine pce = {
	MACHINE_PCE,		/* type */
	"PCEAS",		/* asm_name */
	PCE_ASM_VERSION,	/* asm_title */
	".pce",			/* rom_ext */
	"PCE_INCLUDE",		/* include_env */
	defdirs_pce,		/* default_dirs */
	0xD8,			/* zp_limit */
	0x2000,			/* ram_limit */
	0x2000,			/* ram_base */
	1,			/* ram_page */
	0xF8,			/* ram_bank */
	r65c02_inst,		/* base_inst */
	huc6280_inst,		/* plus_inst */
	pce_pseudo,		/* pseudo_inst */
	pce_pack_8x8_tile,	/* pack_8x8_tile */
	pce_pack_16x16_tile,	/* pack_16x16_tile */
	pce_pack_16x16_sprite,	/* pack_16x16_sprite */
	pce_write_header	/* write_header */
};

