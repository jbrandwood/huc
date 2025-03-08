#ifndef _hucc_old_map_h
#define _hucc_old_map_h

/****************************************************************************
; ***************************************************************************
;
; hucc-old-map.h
;
; Based on the original HuC and MagicKit functions by David Michel and the
; other original HuC developers.
;
; Modifications copyright John Brandwood 2024.
;
; Distributed under the Boost Software License, Version 1.0.
; (See accompanying file LICENSE_1_0.txt or copy at
;  http://www.boost.org/LICENSE_1_0.txt)
;
; ***************************************************************************
; **************************************************************************/

// *************
// Functions in hucc-old-map.asm ...
// *************

#ifdef __HUCC__

#asmdef	HUCC_USES_OLD_MAP 1

extern void __fastcall set_tile_address( unsigned int vram<_di> );
extern void __fastcall sgx_set_tile_address( unsigned int vram<_di> );

extern void __fastcall __nop set_tile_data( unsigned char __far *tiles<vdc_tile_bank:vdc_tile_addr>, unsigned char num_tiles<vdc_num_tiles>, unsigned char __far *palette_table<vdc_attr_bank:vdc_attr_addr>, unsigned char tile_type<vdc_tile_type> );
extern void __fastcall __nop sgx_set_tile_data( unsigned char __far *tiles<sgx_tile_bank:sgx_tile_addr>, unsigned char num_tiles<sgx_num_tiles>, unsigned char __far *palette_table<sgx_attr_bank:sgx_attr_addr>, unsigned char tile_type<sgx_tile_type> );

extern void __fastcall __nop set_far_tile_data( unsigned char tile_bank<vdc_tile_bank>, unsigned char *tile_addr<vdc_tile_addr>, unsigned char num_tiles<vdc_num_tiles>, unsigned char palette_table_bank<vdc_attr_bank>, unsigned char *palette_table_addr<vdc_attr_addr>, unsigned char tile_type<vdc_tile_type> );
extern void __fastcall __nop sgx_set_far_tile_data( unsigned char tile_bank<sgx_tile_bank>, unsigned char *tile_addr<sgx_tile_addr>, unsigned char num_tiles<sgx_num_tiles>, unsigned char palette_table_bank<sgx_attr_bank>, unsigned char *palette_table_addr<sgx_attr_addr>, unsigned char tile_type<sgx_tile_type> );

extern void __fastcall __nop set_map_data( unsigned char __far *map<vdc_map_bank:vdc_map_addr>, unsigned char w<vdc_map_width>, unsigned char h<vdc_map_height> );
extern void __fastcall __nop sgx_set_map_data( unsigned char __far *map<sgx_map_bank:sgx_map_addr>, unsigned char w<sgx_map_width>, unsigned char h<sgx_map_height> );

extern void __fastcall __nop set_far_map_data( unsigned char map_bank<vdc_map_bank>, unsigned char *map_addr<vdc_map_addr>, unsigned char w<vdc_map_width>, unsigned char h<vdc_map_height> );
extern void __fastcall __nop sgx_set_far_map_data( unsigned char map_bank<vdc_map_bank>, unsigned char *map_addr<vdc_map_addr>, unsigned char w<sgx_map_width>, unsigned char h<sgx_map_height> );

extern void __fastcall load_tile( unsigned int vram<_di> );
extern void __fastcall sgx_load_tile( unsigned int vram<_di> );

extern void __fastcall load_map( unsigned char bat_x<_al>, unsigned char bat_y<_ah>, int map_x<_bx>, int map_y<_dx>, unsigned char tiles_w<_cl>, unsigned char tiles_h<_ch> );
extern void __fastcall sgx_load_map( unsigned char bat_x<_al>, unsigned char bat_y<_ah>, int map_x<_bx>, int map_y<_dx>, unsigned char tiles_w<_cl>, unsigned char tiles_h<_ch> );

extern unsigned char __fastcall map_get_tile( unsigned char map_x<_bl>, unsigned char map_y<_bh> );
extern unsigned char __fastcall sgx_map_get_tile( unsigned char map_x<_bl>, unsigned char map_y<_bh> );

extern void __fastcall map_put_tile( unsigned char map_x<_bl>, unsigned char map_y<_bh>, unsigned char tile<_al> );
extern void __fastcall sgx_map_put_tile( unsigned char map_x<_bl>, unsigned char map_y<_bh>, unsigned char tile<_al> );

extern void __fastcall put_tile( unsigned char tile<_bl>, unsigned char bat_x<_al>, unsigned char bat_y<_ah> );
extern void __fastcall sgx_put_tile( unsigned char tile<_bl>, unsigned char bat_x<_al>, unsigned char bat_y<_ah> );

// Deprecated functions ...

extern void __fastcall set_tile_data( unsigned char *tile_ex<_di> );

extern void __fastcall __nop set_map_pals( unsigned char __far *palette_table<vdc_attr_bank:vdc_attr_addr> );
extern void __fastcall __nop set_map_tile_type( unsigned char tile_type<vdc_tile_type> );

extern void __fastcall __nop sgx_set_map_pals( unsigned char __far *palette_table<sgx_attr_bank:sgx_attr_addr> );
extern void __fastcall __nop sgx_set_map_tile_type( unsigned char tile_type<sgx_tile_type> );

extern void __fastcall set_map_tile_base( unsigned int vram<_di> );
extern void __fastcall sgx_set_map_tile_base( unsigned int vram<_di> );

#endif // __HUCC__

#endif // _hucc_old_map_h
