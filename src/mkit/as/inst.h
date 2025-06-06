/* MOS 6502 instruction table */
/* *INDENT-OFF* */
struct t_opcode m6502_inst[57] = {
	{NULL, "ADC", class4, IMM|ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x61, 0},
	{NULL, "AND", class4, IMM|ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x21, 0},
	{NULL, "ASL", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x02, 0},
	{NULL, "BCC", class2, 0, 0x90, 0},
	{NULL, "BCS", class2, 0, 0xB0, 0},
	{NULL, "BEQ", class2, 0, 0xF0, 0},
	{NULL, "BIT", class4, ZP|ABS, 0x00, 2},
	{NULL, "BMI", class2, 0, 0x30, 0},
	{NULL, "BNE", class2, 0, 0xD0, 0},
	{NULL, "BPL", class2, 0, 0x10, 0},
	{NULL, "BRK", class1, 0, 0x00, 0},
	{NULL, "BVC", class2, 0, 0x50, 0},
	{NULL, "BVS", class2, 0, 0x70, 0},
	{NULL, "CLC", class1, 0, 0x18, 0},
	{NULL, "CLD", class1, 0, 0xD8, 0},
	{NULL, "CLI", class1, 0, 0x58, 0},
	{NULL, "CLV", class1, 0, 0xB8, 0},
	{NULL, "CMP", class4, IMM|ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0xC1, 0},
	{NULL, "CPX", class4, IMM|ZP|ABS, 0xE0, 1},
	{NULL, "CPY", class4, IMM|ZP|ABS, 0xC0, 1},
	{NULL, "DEC", class4, ZP|ZP_X|ABS|ABS_X, 0x00, 3},
	{NULL, "DEX", class1, 0, 0xCA, 0},
	{NULL, "DEY", class1, 0, 0x88, 0},
	{NULL, "EOR", class4, IMM|ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x41, 0},
	{NULL, "INC", class4, ZP|ZP_X|ABS|ABS_X, 0x00, 4},
	{NULL, "INX", class1, 0, 0xE8, 0},
	{NULL, "INY", class1, 0, 0xC8, 0},
	{NULL, "JMP", class4, ABS|ABS_IND, 0x40, 0},
//	{NULL, "JSR", class4, ABS, 0x14, 0},
	{NULL, "JSR", classC, ABS, 0x14, 0},
	{NULL, "LDA", class4, IMM|ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0xA1, 0},
	{NULL, "LDX", class4, IMM|ZP|ZP_Y|ABS|ABS_Y, 0xA2, 1},
	{NULL, "LDY", class4, IMM|ZP|ZP_X|ABS|ABS_X, 0xA0, 1},
	{NULL, "LSR", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x42, 0},
	{NULL, "NOP", class1, 0, 0xEA, 0},
	{NULL, "ORA", class4, IMM|ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x01, 0},
	{NULL, "PHA", class1, 0, 0x48, 0},
	{NULL, "PHP", class1, 0, 0x08, 0},
	{NULL, "PLA", class1, 0, 0x68, 0},
	{NULL, "PLP", class1, 0, 0x28, 0},
	{NULL, "ROL", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x22, 0},
	{NULL, "ROR", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x62, 0},
	{NULL, "RTI", class1, 0, 0x40, 0},
//	{NULL, "RTS", class1, 0, 0x60, 0},
	{NULL, "RTS", classR, 0, 0x60, 0},
	{NULL, "SBC", class4, IMM|ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0xE1, 0},
	{NULL, "SEC", class1, 0, 0x38, 0},
	{NULL, "SED", class1, 0, 0xF8, 0},
	{NULL, "SEI", class1, 0, 0x78, 0},
	{NULL, "STA", class4, ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x81, 0},
	{NULL, "STX", class4, ZP|ZP_Y|ABS, 0x82, 0},
	{NULL, "STY", class4, ZP|ZP_X|ABS, 0x80, 0},
	{NULL, "TAX", class1, 0, 0xAA, 0},
	{NULL, "TAY", class1, 0, 0xA8, 0},
	{NULL, "TSX", class1, 0, 0xBA, 0},
	{NULL, "TXA", class1, 0, 0x8A, 0},
	{NULL, "TXS", class1, 0, 0x9A, 0},
	{NULL, "TYA", class1, 0, 0x98, 0},
	{NULL, NULL, NULL, 0, 0, 0}
};

/* MOS 6502 undocumented instruction table */
/* *INDENT-OFF* */
struct t_opcode undoc_inst[13] = {
	{NULL, "ANC", class4, IMM, 0x03, 0},
	{NULL, "ALR", class4, IMM, 0x43, 0},
	{NULL, "ARR", class4, IMM, 0x63, 0},
	{NULL, "AXS", class4, IMM, 0xC3, 0},
	{NULL, "DCP", class4, ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0xC3, 0},
	{NULL, "ISC", class4, ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0xE3, 0},
	{NULL, "LAX", class4, ZP|ZP_Y|ZP_IND_X|ZP_IND_Y|ABS|ABS_Y, 0xA3, 1},
	{NULL, "RLA", class4, ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x23, 0},
	{NULL, "RRA", class4, ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x63, 0},
	{NULL, "SAX", class4, ZP|ZP_Y|ZP_IND_X|ABS, 0x83, 0},
	{NULL, "SLO", class4, ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x03, 0},
	{NULL, "SRE", class4, ZP|ZP_X|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x43, 0},
	{NULL, NULL, NULL, 0, 0, 0}
};

/* Rockwell 65C02 instruction table */
/* *INDENT-OFF* */
struct t_opcode r65c02_inst[101] = {
	{NULL, "ADC", class4, IMM|ZP|ZP_X|ZP_IND|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x61, 0},
	{NULL, "AND", class4, IMM|ZP|ZP_X|ZP_IND|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x21, 0},
	{NULL, "ASL", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x02, 0},
	{NULL, "BBR",  class10,0, 0x0F, 0},
	{NULL, "BBR0", class5, 0, 0x0F, 0},
	{NULL, "BBR1", class5, 0, 0x1F, 0},
	{NULL, "BBR2", class5, 0, 0x2F, 0},
	{NULL, "BBR3", class5, 0, 0x3F, 0},
	{NULL, "BBR4", class5, 0, 0x4F, 0},
	{NULL, "BBR5", class5, 0, 0x5F, 0},
	{NULL, "BBR6", class5, 0, 0x6F, 0},
	{NULL, "BBR7", class5, 0, 0x7F, 0},
	{NULL, "BBS",  class10,0, 0x8F, 0},
	{NULL, "BBS0", class5, 0, 0x8F, 0},
	{NULL, "BBS1", class5, 0, 0x9F, 0},
	{NULL, "BBS2", class5, 0, 0xAF, 0},
	{NULL, "BBS3", class5, 0, 0xBF, 0},
	{NULL, "BBS4", class5, 0, 0xCF, 0},
	{NULL, "BBS5", class5, 0, 0xDF, 0},
	{NULL, "BBS6", class5, 0, 0xEF, 0},
	{NULL, "BBS7", class5, 0, 0xFF, 0},
	{NULL, "BCC", class2, 0, 0x90, 0},
	{NULL, "BCS", class2, 0, 0xB0, 0},
	{NULL, "BEQ", class2, 0, 0xF0, 0},
	{NULL, "BIT", class4, IMM|ZP|ZP_X|ABS|ABS_X, 0x00, 2},
	{NULL, "BMI", class2, 0, 0x30, 0},
	{NULL, "BNE", class2, 0, 0xD0, 0},
	{NULL, "BPL", class2, 0, 0x10, 0},
	{NULL, "BRA", class2, 0, 0x80, 0},
	{NULL, "BRK", class1, 0, 0x00, 0},
	{NULL, "BVC", class2, 0, 0x50, 0},
	{NULL, "BVS", class2, 0, 0x70, 0},
	{NULL, "CLC", class1, 0, 0x18, 0},
	{NULL, "CLD", class1, 0, 0xD8, 0},
	{NULL, "CLI", class1, 0, 0x58, 0},
	{NULL, "CLV", class1, 0, 0xB8, 0},
	{NULL, "CMP", class4, IMM|ZP|ZP_X|ZP_IND|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0xC1, 0},
	{NULL, "CPX", class4, IMM|ZP|ABS, 0xE0, 1},
	{NULL, "CPY", class4, IMM|ZP|ABS, 0xC0, 1},
	{NULL, "DEC", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x00, 3},
	{NULL, "DEX", class1, 0, 0xCA, 0},
	{NULL, "DEY", class1, 0, 0x88, 0},
	{NULL, "EOR", class4, IMM|ZP|ZP_X|ZP_IND|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x41, 0},
	{NULL, "INC", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x00, 4},
	{NULL, "INX", class1, 0, 0xE8, 0},
	{NULL, "INY", class1, 0, 0xC8, 0},
//	{NULL, "JMP", class4, ABS|ABS_IND|ABS_IND_X, 0x40, 0},
//	{NULL, "JSR", class4, ABS, 0x14, 0},
	{NULL, "JMP", classC, ABS|ABS_IND|ABS_IND_X, 0x40, 0},
	{NULL, "JSR", classC, ABS, 0x14, 0},
	{NULL, "LDA", class4, IMM|ZP|ZP_X|ZP_IND|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0xA1, 0},
	{NULL, "LDX", class4, IMM|ZP|ZP_Y|ABS|ABS_Y, 0xA2, 1},
	{NULL, "LDY", class4, IMM|ZP|ZP_X|ABS|ABS_X, 0xA0, 1},
	{NULL, "LSR", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x42, 0},
	{NULL, "NOP", class1, 0, 0xEA, 0},
	{NULL, "ORA", class4, IMM|ZP|ZP_X|ZP_IND|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x01, 0},
	{NULL, "PHA", class1, 0, 0x48, 0},
	{NULL, "PHP", class1, 0, 0x08, 0},
	{NULL, "PHX",  class1, 0, 0xDA, 0},
	{NULL, "PHY",  class1, 0, 0x5A, 0},
	{NULL, "PLA", class1, 0, 0x68, 0},
	{NULL, "PLP", class1, 0, 0x28, 0},
	{NULL, "PLX",  class1, 0, 0xFA, 0},
	{NULL, "PLY",  class1, 0, 0x7A, 0},
	{NULL, "RMB",  class9, 0, 0x07, 0},
	{NULL, "RMB0", class4, ZP, 0x03, 0},
	{NULL, "RMB1", class4, ZP, 0x13, 0},
	{NULL, "RMB2", class4, ZP, 0x23, 0},
	{NULL, "RMB3", class4, ZP, 0x33, 0},
	{NULL, "RMB4", class4, ZP, 0x43, 0},
	{NULL, "RMB5", class4, ZP, 0x53, 0},
	{NULL, "RMB6", class4, ZP, 0x63, 0},
	{NULL, "RMB7", class4, ZP, 0x73, 0},
	{NULL, "ROL", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x22, 0},
	{NULL, "ROR", class4, ACC|ZP|ZP_X|ABS|ABS_X, 0x62, 0},
	{NULL, "RTI", class1, 0, 0x40, 0},
//	{NULL, "RTS", class1, 0, 0x60, 0},
	{NULL, "RTS", classR, 0, 0x60, 0},
	{NULL, "SBC", class4, IMM|ZP|ZP_X|ZP_IND|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0xE1, 0},
	{NULL, "SEC", class1, 0, 0x38, 0},
	{NULL, "SED", class1, 0, 0xF8, 0},
	{NULL, "SEI", class1, 0, 0x78, 0},
	{NULL, "SMB",  class9, 0, 0x87, 0},
	{NULL, "SMB0", class4, ZP, 0x83, 0},
	{NULL, "SMB1", class4, ZP, 0x93, 0},
	{NULL, "SMB2", class4, ZP, 0xA3, 0},
	{NULL, "SMB3", class4, ZP, 0xB3, 0},
	{NULL, "SMB4", class4, ZP, 0xC3, 0},
	{NULL, "SMB5", class4, ZP, 0xD3, 0},
	{NULL, "SMB6", class4, ZP, 0xE3, 0},
	{NULL, "SMB7", class4, ZP, 0xF3, 0},
	{NULL, "STA", class4, ZP|ZP_X|ZP_IND|ZP_IND_X|ZP_IND_Y|ABS|ABS_X|ABS_Y, 0x81, 0},
	{NULL, "STX", class4, ZP|ZP_Y|ABS, 0x82, 0},
	{NULL, "STY", class4, ZP|ZP_X|ABS, 0x80, 0},
	{NULL, "STZ", class4, ZP|ZP_X|ABS|ABS_X, 0x00, 0x05},
	{NULL, "TAX", class1, 0, 0xAA, 0},
	{NULL, "TAY", class1, 0, 0xA8, 0},
	{NULL, "TRB", class4, ZP|ABS, 0x10, 0},
	{NULL, "TSB", class4, ZP|ABS, 0x00, 0},
	{NULL, "TSX", class1, 0, 0xBA, 0},
	{NULL, "TXA", class1, 0, 0x8A, 0},
	{NULL, "TXS", class1, 0, 0x9A, 0},
	{NULL, "TYA", class1, 0, 0x98, 0},
	{NULL, NULL, NULL, 0, 0, 0}
};

/* WDC 65C02 additional instructions (on top of Rockwell's r65C02) */
/* *INDENT-OFF* */
struct t_opcode w65c02_inst[3] = {
	{NULL, "STP", class1, 0, 0xDB, 0},
	{NULL, "WAI", class1, 0, 0xCB, 0},
	{NULL, NULL, NULL, 0, 0, 0}
};

/* pseudo instruction table */
struct t_opcode base_pseudo[] = {
	{NULL,  "*",            do_star,        PSEUDO, P_ORG,     1},
	{NULL,  "=",            do_equ,         PSEUDO, P_EQU,     0},
	{NULL,  "CALL",         do_call,        PSEUDO, P_CALL,    0},

	{NULL,  "ALIGN",        do_align,       PSEUDO, P_ALIGN,   0},
	{NULL,  "BANK",         do_bank,        PSEUDO, P_BANK,    0},
	{NULL,  "BSS",          do_section,     PSEUDO, P_BSS,     S_BSS},
//	{NULL,  "BYTE",         do_db,          PSEUDO, P_DB,      0},
	{NULL,  "CODE",         do_section,     PSEUDO, P_CODE,    S_CODE},
	{NULL,  "DATA",         do_section,     PSEUDO, P_DATA,    S_DATA},
	{NULL,  "DB",           do_db,          PSEUDO, P_DB,      0},
	{NULL,  "DD",           do_dd,          PSEUDO, P_DD,      0},
	{NULL,  "DS",           do_ds,          PSEUDO, P_DS,      0},
	{NULL,  "DW",           do_dw,          PSEUDO, P_DW,      0},
	{NULL,  "DWH",          do_dwh,         PSEUDO, P_DWH,     0},
	{NULL,  "DWL",          do_dwl,         PSEUDO, P_DWL,     0},
//	{NULL,  "DWORD",        do_dd,          PSEUDO, P_DD,      0},
	{NULL,  "ELSE",         do_else,        PSEUDO, P_ELSE,    0},
	{NULL,  "ENDIF",        do_endif,       PSEUDO, P_ENDIF,   0},
	{NULL,  "ENDMACRO",     do_endm,        PSEUDO, P_ENDM,    0},
	{NULL,  "ENDM",         do_endm,        PSEUDO, P_ENDM,    0},
	{NULL,  "ENDP",         do_endp,        PSEUDO, P_ENDP,    P_PROC},
	{NULL,  "ENDPROCGROUP", do_endp,        PSEUDO, P_ENDPG,   P_PGROUP},
	{NULL,  "ENDS",         do_ends,        PSEUDO, P_ENDS,    0},
	{NULL,  "EQU",          do_equ,         PSEUDO, P_EQU,     0},
	{NULL,  "FAIL",         do_fail,        PSEUDO, P_FAIL,    0},
	{NULL,  "FUNC",         do_func,        PSEUDO, P_FUNC,    0},
	{NULL,  "IF",           do_if,          PSEUDO, P_IF,      0},
	{NULL,  "IFDEF",        do_ifdef,       PSEUDO, P_IFDEF,   1},
	{NULL,  "IFNDEF",       do_ifdef,       PSEUDO, P_IFNDEF,  0},
	{NULL,  "INCBIN",       do_incbin,      PSEUDO, P_INCBIN,  0},
	{NULL,  "INCLUDE",      do_include,     PSEUDO, P_INCLUDE, 0},
	{NULL,  "INCCHR",       do_incchr,      PSEUDO, P_INCCHR,  0},
	{NULL,  "PADCHR",       do_incchr,      PSEUDO, P_INCCHR,  1},
	{NULL,  "LIST",         do_list,        PSEUDO, P_LIST,    0},
	{NULL,  "MAC",          do_macro,       PSEUDO, P_MACRO,   0},
	{NULL,  "MACRO",        do_macro,       PSEUDO, P_MACRO,   0},
	{NULL,  "MLIST",        do_mlist,       PSEUDO, P_MLIST,   0},
	{NULL,  "NOLIST",       do_nolist,      PSEUDO, P_NOLIST,  0},
	{NULL,  "NOMLIST",      do_nomlist,     PSEUDO, P_NOMLIST, 0},
	{NULL,  "OPT",          do_opt,         PSEUDO, P_OPT,     0},
	{NULL,  "ORG",          do_org,         PSEUDO, P_ORG,     0},
	{NULL,  "OUTBIN",       do_outbin,      PSEUDO, P_OUTBIN,  0},
	{NULL,  "OUTZX0",       do_outbin,      PSEUDO, P_OUTBIN,  1},
	{NULL,  "PAGE",         do_page,        PSEUDO, P_PAGE,    0},
	{NULL,  "PROC",         do_proc,        PSEUDO, P_PROC,    P_PROC},
	{NULL,  "PROCGROUP",    do_proc,        PSEUDO, P_PGROUP,  P_PGROUP},
	{NULL,  "RSSET",        do_rsset,       PSEUDO, P_RSSET,   0},
	{NULL,  "RS",           do_rs,          PSEUDO, P_RS,      0},
	{NULL,  "STRUCT",       do_struct,      PSEUDO, P_STRUCT,  0},
//	{NULL,  "WORD",         do_dw,          PSEUDO, P_DW,      0},
	{NULL,  "ZP",           do_section,     PSEUDO, P_ZP,      S_ZP},

	{NULL, ".ALIGN",        do_align,       PSEUDO, P_ALIGN,   0},
	{NULL, ".BANK",         do_bank,        PSEUDO, P_BANK,    0},
	{NULL, ".BSS",          do_section,     PSEUDO, P_BSS,     S_BSS},
	{NULL, ".BYTE",         do_db,          PSEUDO, P_DB,      0},
	{NULL, ".CODE",         do_section,     PSEUDO, P_CODE,    S_CODE},
	{NULL, ".DATA",         do_section,     PSEUDO, P_DATA,    S_DATA},
	{NULL, ".DB",           do_db,          PSEUDO, P_DB,      0},
	{NULL, ".DD",           do_dd,          PSEUDO, P_DD,      0},
	{NULL, ".DS",           do_ds,          PSEUDO, P_DS,      0},
	{NULL, ".DW",           do_dw,          PSEUDO, P_DW,      0},
	{NULL, ".DWH",          do_dwh,         PSEUDO, P_DWH,     0},
	{NULL, ".DWL",          do_dwl,         PSEUDO, P_DWL,     0},
	{NULL, ".DWORD",        do_dd,          PSEUDO, P_DD,      0},
	{NULL, ".ELSE",         do_else,        PSEUDO, P_ELSE,    0},
	{NULL, ".ENDIF",        do_endif,       PSEUDO, P_ENDIF,   0},
	{NULL, ".ENDMACRO",     do_endm,        PSEUDO, P_ENDM,    0},
	{NULL, ".ENDM",         do_endm,        PSEUDO, P_ENDM,    0},
	{NULL, ".ENDP",         do_endp,        PSEUDO, P_ENDP,    P_PROC},
	{NULL, ".ENDPROCGROUP", do_endp,        PSEUDO, P_ENDPG,   P_PGROUP},
	{NULL, ".EQU",          do_equ,         PSEUDO, P_EQU,     0},
	{NULL, ".FAIL",         do_fail,        PSEUDO, P_FAIL,    0},
	{NULL, ".FUNC",         do_func,        PSEUDO, P_FUNC,    0},
	{NULL, ".HOME",         do_section,     PSEUDO, P_CODE,    S_HOME},
	{NULL, ".IF",           do_if,          PSEUDO, P_IF,      0},
	{NULL, ".IFDEF",        do_ifdef,       PSEUDO, P_IFDEF,   1},
	{NULL, ".IFNDEF",       do_ifdef,       PSEUDO, P_IFNDEF,  0},
	{NULL, ".INCBIN",       do_incbin,      PSEUDO, P_INCBIN,  0},
	{NULL, ".INCLUDE",      do_include,     PSEUDO, P_INCLUDE, 0},
	{NULL, ".INCCHR",       do_incchr,      PSEUDO, P_INCCHR,  0},
	{NULL, ".PADCHR",       do_incchr,      PSEUDO, P_INCCHR,  1},
	{NULL, ".LIST",         do_list,        PSEUDO, P_LIST,    0},
	{NULL, ".MAC",          do_macro,       PSEUDO, P_MACRO,   0},
	{NULL, ".MACRO",        do_macro,       PSEUDO, P_MACRO,   0},
	{NULL, ".MLIST",        do_mlist,       PSEUDO, P_MLIST,   0},
	{NULL, ".NOLIST",       do_nolist,      PSEUDO, P_NOLIST,  0},
	{NULL, ".NOMLIST",      do_nomlist,     PSEUDO, P_NOMLIST, 0},
	{NULL, ".OPT",          do_opt,         PSEUDO, P_OPT,     0},
	{NULL, ".ORG",          do_org,         PSEUDO, P_ORG,     0},
	{NULL, ".OUTBIN",       do_outbin,      PSEUDO, P_OUTBIN,  0},
	{NULL, ".OUTZX0",       do_outbin,      PSEUDO, P_OUTBIN,  1},
	{NULL, ".PAGE",         do_page,        PSEUDO, P_PAGE,    0},
	{NULL, ".PROC",         do_proc,        PSEUDO, P_PROC,    P_PROC},
	{NULL, ".PROCGROUP",    do_proc,        PSEUDO, P_PGROUP,  P_PGROUP},
	{NULL, ".RSSET",        do_rsset,       PSEUDO, P_RSSET,   0},
	{NULL, ".RS",           do_rs,          PSEUDO, P_RS,      0},
	{NULL, ".SET",          do_equ,         PSEUDO, P_EQU,     1},
	{NULL, ".STRUCT",       do_struct,      PSEUDO, P_STRUCT,  0},
	{NULL, ".ENDS",         do_ends,        PSEUDO, P_ENDS,    0},
	{NULL, ".WORD",         do_dw,          PSEUDO, P_DW,      0},
	{NULL, ".ZP",           do_section,     PSEUDO, P_ZP,      S_ZP},

	{NULL, ".ZEROPAGE",     do_section,     PSEUDO, P_ZP,      S_ZP},
	{NULL, ".3PASS",        do_3pass,       PSEUDO, P_3PASS,   0},
	{NULL, ".ALIAS",        do_alias,       PSEUDO, P_ALIAS,   0},
	{NULL, ".REF",          do_ref,         PSEUDO, P_REF,     0},
	{NULL, ".PHASE",        do_phase,       PSEUDO, P_PHASE,   0},
	{NULL, ".DEPHASE",      do_phase,       PSEUDO, P_PHASE,   1},
	{NULL, ".DBG",          do_debug,       PSEUDO, P_DEBUG,   0},

	// pseudo-ops that support asm code generated by KickC

	{NULL, ".PCEAS",        do_kickc,       PSEUDO, P_KICKC,   0},
	{NULL, ".KICKC",        do_kickc,       PSEUDO, P_KICKC,   1},
	{NULL, ".CPU",          do_ignore,      PSEUDO, P_IGNORE,  0},
	{NULL, ".ENCODING",     do_ignore,      PSEUDO, P_IGNORE,  0},
	{NULL, ".SEGMENT",      do_segment,     PSEUDO, P_SEGMENT, 0},
	{NULL, ".CONST",        do_label,       PSEUDO, P_LABEL,   0},
	{NULL, ".LABEL",        do_label,       PSEUDO, P_LABEL,   0},
	{NULL, ".VAR",          do_label,       PSEUDO, P_LABEL,   1},
	{NULL, ".TEXT",         do_db,          PSEUDO, P_DB,      1},
	{NULL, ".FILL",         do_ds,          PSEUDO, P_DS,      0},
	{NULL, "{",             do_proc,        PSEUDO, P_PROC,    P_KICKC},
	{NULL, "}",             do_endp,        PSEUDO, P_ENDP,    P_KICKC},

	// pseudo-ops that support asm code generated by SDCC

	{NULL, ".MODULE",       do_include,     PSEUDO, P_INCLUDE, 1},
	{NULL, ".OPTSDCC",      do_ignore,      PSEUDO, P_IGNORE,  0},
	{NULL, ".R6502",        do_kickc,       PSEUDO, P_KICKC,   2},
	{NULL, ".R65C02",       do_kickc,       PSEUDO, P_KICKC,   2},
	{NULL, ".GLOBL",        do_ignore,      PSEUDO, P_IGNORE,  0},
	{NULL, ".AREA",         do_segment,     PSEUDO, P_SEGMENT, 1},
	{NULL, ".ASCII",        do_db,          PSEUDO, P_DB,      2},

	// pseudo-ops that support asm code generated by HuCC

	{NULL, ".HUCC",         do_kickc,       PSEUDO, P_KICKC,   4},

	{NULL, NULL, NULL, 0, 0, 0}
};
/* *INDENT-ON* */
