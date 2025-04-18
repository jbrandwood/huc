#ifndef _hucc_chrmap_h
#define _hucc_chrmap_h

/****************************************************************************
; ***************************************************************************
;
; hucc-chrmap.h
;
; A simple map system based on 8x8 characters (aka "tiles") in BAT format.
;
; Copyright John Brandwood 2025.
;
; Distributed under the Boost Software License, Version 1.0.
; (See accompanying file LICENSE_1_0.txt or copy at
;  http://www.boost.org/LICENSE_1_0.txt)
;
; ***************************************************************************
; ***************************************************************************
;
; The maximum X and Y size for chrmaps is 256 characters (2048 pixels).
;
; The maximum total size for a chrmap is 16KBytes, which allows for maps up
; to 256x32 tiles (2048x256 pixels).
;
; ***************************************************************************
; **************************************************************************/

// *************
// This code extends the blkmap library ...
// *************

#include "hucc-blkmap.h"

// *************
// Functions in chrmap.asm ...
// *************

#ifdef __HUCC__

#asmdef	HUCC_USES_CHRMAP 1

extern void __fastcall __nop set_chrmap( unsigned char __far *chrmap<vdc_map_bank:vdc_map_addr>, unsigned char tiles_w<vdc_map_line_w> );

extern void __fastcall draw_bat( void );
extern void __fastcall scroll_bat( void );

extern void __fastcall blit_bat( unsigned char tile_x<map_bat_x>, unsigned char tile_y<map_bat_y>, unsigned char tile_w<map_draw_x>, unsigned char tile_h<map_draw_y> );

#ifdef _SGX

extern void __fastcall __nop sgx_set_chrmap( unsigned char __far *chrmap<sgx_map_bank:sgx_map_addr>, unsigned char tiles_w<sgx_map_line_w> );

extern void __fastcall sgx_draw_bat( void );
extern void __fastcall sgx_scroll_bat( void );

extern void __fastcall sgx_blit_bat( unsigned char tile_x<map_bat_x>, unsigned char tile_y<map_bat_y>, unsigned char tile_w<map_draw_x>, unsigned char tile_h<map_draw_y> );

#endif

#endif // __HUCC__

#endif // _hucc_chrmap_h
