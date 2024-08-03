#ifndef _hucc_baselib_h
#define _hucc_baselib_h

/****************************************************************************
; ***************************************************************************
;
; hucc-baselib.h
;
; Basic library functions provided as macros.
;
; Copyright John Brandwood 2024.
;
; Distributed under the Boost Software License, Version 1.0.
; (See accompanying file LICENSE_1_0.txt or copy at
;  http://www.boost.org/LICENSE_1_0.txt)
;
; ***************************************************************************
; **************************************************************************/

// *************
// Joypad defines ...
// *************

#define	JOY_A		0x01
#define	JOY_I		0x01
#define	JOY_B		0x02
#define	JOY_II		0x02
#define	JOY_SLCT	0x04
#define	JOY_SEL		0x04
#define	JOY_STRT	0x08
#define	JOY_RUN		0x08
#define	JOY_UP		0x10
#define	JOY_RGHT	0x20
#define	JOY_RIGHT	0x20
#define	JOY_DOWN	0x40
#define	JOY_LEFT	0x80

#define	JOY_C		0x0100
#define	JOY_III		0x0100
#define	JOY_D		0x0200
#define	JOY_IV		0x0200
#define	JOY_E		0x0400
#define	JOY_V		0x0400
#define	JOY_F		0x0800
#define	JOY_VI		0x0800

#define JOY_SIXBUT	0x8000
#define JOY_TYPE6	0x8000

// *************
// System Card variables ...
// *************

extern unsigned int si;
extern unsigned int di;
extern unsigned int bp;

extern unsigned int ax;
extern unsigned int bx;
extern unsigned int cx;
extern unsigned int dx;

extern unsigned char al;
extern unsigned char ah;
extern unsigned char bl;
extern unsigned char bh;
extern unsigned char cl;
extern unsigned char ch;
extern unsigned char dl;
extern unsigned char dh;

// *************
// Functions in hucc-baselib.asm ...
// *************

#ifdef __HUCC__

#asmdef	HUCC_USES_BASELIB 1

extern unsigned int __fastcall __xsafe __macro peek( unsigned int addr<__ptr> );
extern unsigned int __fastcall __xsafe __macro peekw( unsigned int addr<__ptr> );
extern void __fastcall __xsafe __macro poke( unsigned int addr<__ptr>, unsigned char with<acc> );
extern void __fastcall __xsafe __macro pokew( unsigned int addr<__ptr>, unsigned int with<acc> );

extern unsigned int __fastcall __xsafe __farpeekw( void __far *addr<__fbank:__fptr> );

extern void __fastcall __xsafe __macro vsync( void );
extern void __fastcall __xsafe __macro vsync( unsigned char count<acc> );

extern void __fastcall __xsafe __macro joy( unsigned char which<acc> );
extern void __fastcall __xsafe __macro joytrg( unsigned char which<acc> );

extern void __fastcall __xsafe __nop set_color( unsigned int index<VCE_CTA>, unsigned int value<VCE_CTW> );
extern void __fastcall __xsafe set_color_rgb( unsigned int index<VCE_CTA>, unsigned char r<_al>, unsigned char g<_ah>, unsigned char b<acc> );
extern void __fastcall __xsafe __macro get_color( unsigned int index<VCE_CTA> );

extern unsigned char __fastcall __xsafe __macro clock_hh( void );
extern unsigned char __fastcall __xsafe __macro clock_mm( void );
extern unsigned char __fastcall __xsafe __macro clock_ss( void );
extern unsigned char __fastcall __xsafe __macro clock_tt( void );
extern void __fastcall __xsafe __macro clock_reset( void );

extern void __fastcall __xsafe srand( unsigned char seed<acc> );
extern unsigned char __fastcall __xsafe random( unsigned char limit<acc> );
extern unsigned char __fastcall __xsafe rand( void );

extern unsigned char __fastcall __xsafe __macro cd_execoverlay( unsigned char ovl_index<acc> );
extern unsigned char __fastcall __xsafe __macro ac_exists( void );

extern unsigned int __fastcall __xsafe abs( int value<acc> );

// Functions that are only implemented in the TGEMU emulator for unit-testing
// the compiler and which should never be used in normal HuCC projects ...

extern void __fastcall __xsafe dump_screen( void );
extern void __fastcall __xsafe abort( void );
extern void __fastcall __xsafe exit( int value<acc> );

#endif // __HUCC__

#endif // _hucc_baselib_h
