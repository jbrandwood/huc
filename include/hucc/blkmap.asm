; ***************************************************************************
; ***************************************************************************
;
; blkmap.asm
;
; A map system based on 16x16 meta-tiles (aka "blocks", aka CHR/BLK/MAP).
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
; The maximum X and Y size for blkmaps is 128 blocks (2048 pixels).
;
; The maximum total size for a blkmap is 16KBytes, which allows for maps up
; to 128x128 blocks (2048x2048 pixels).
;
; Huge multi-screen blkmaps are also supported (optionally).
;
; The maximum X and Y for multi-screen maps is 128 screens (32768 pixels).
;
; The maximum total size for a multi-screen map is 8KBytes, which allows for
; a total of 1024 screens.
;
; ***************************************************************************
; ***************************************************************************

;
; Include dependancies ...
;

		include "common.asm"		; Common helpers.
		include "vce.asm"		; Useful VCE routines.
		include "vdc.asm"		; Useful VCE routines.

;
; Support large blkmaps up to 16KBytes instead of the regular 8KBytes?
;
; The maximum X and Y size for regular blkmaps is 128 tiles (2048 pixels).
;
; This allows for individual maps up to 128x128 tiles (2048x2048 pixels) vs
; default limit of 128x64 or 64x128.
;

	.ifndef	BLKMAP_LARGEMAP
BLKMAP_LARGEMAP	=	0	; (0 or 1)
	.endif

;
; Support huge multi-screen maps, up to 32768 pixels wide/high?
;
; These are sectorized maps split into individual BAT-sized "screens", with
; a maximum of 1024 screens per map.
;
; The BAT size that is used when drawing *must* be the same as the BAT size
; that was chosen when creating the multi-screen map.
;
; Each screen can use a unique set of block definitions, or it might choose
; to share the same block definitions that are used on another screen.
;
; Each screen may choose which 8KByte banks of character data to select for
; the 4 banks (32KBytes) of VRAM that its block definitions use, with a max
; of 16 banks of characters per multi-screen map.
;
; Loading those character banks dynamically is an exercise for the user!
;
; Enabling support adds extra library code, and slightly slows down the use
; of regular blkmaps.
;

	.ifndef	BLKMAP_MULTISCR
BLKMAP_MULTISCR	=	1	; (0 or 1)
	.endif

;
; Block definitions can either use character data from VRAM $1000..$7FFF or
; they can be limited to use the 32KByte of characters in VRAM $1000..$4FFF
; which then frees up 2-bits for flag information for each character in the
; block.
;
; These 2-bits are perfect for using as collision information in game maps,
; allowing storage of states like transparent, solid, up-slope, down-slope.
;
; Typically this flag information is set by the map conversion tools from a
; seperate "collision" map layer.
;

	.ifndef	BLKDEF_CHR_FLAG
BLKDEF_CHR_FLAG	=	1	; (0 or 1)
	.endif

;
; Block definitions are accessed in MPR2 ($4000..$5FFF), and must not cross
; the bank boundary.
;
; When working this way, 8 pointers in ZP are used to access the individual
; bytes in the block definition. This is fine when using a regular map on a
; PC Engine, but it is awfully slow when using multi-screen maps or drawing
; maps on both VDC chips in a SuperGRAFX because the pointer values must be
; constantly changed.
;
; When using multi-screen maps, or when developing a SuperGRAFX game, or if
; developing a CDROM game, then it is usually preferable to store the block
; definitions with 2KByte alignment, especially if they are normally stored
; compressed and then decompressed into a 2KByte buffer in RAM when needed.
;
; This option controls whether the definitions are stored 2KByte aligned or
; if pointers should be used instead.
;

	.ifndef	BLKDEF_POINTERS
BLKDEF_POINTERS	=	0	; (0 or 1)
	.endif

;
; Add a timing bar to the scroll_map() to see how long the drawing takes?
;

	.ifndef	BLKMAP_TIMETEST
BLKMAP_TIMETEST	=	0	; (0 or 1)
	.endif

;
;
;

	.if	BLKDEF_POINTERS

		.zp
blk_tl_l_ptr:	ds	2	; Block definition top left lo-byte.
blk_tr_l_ptr:	ds	2	; Block definition top right lo-byte.
blk_bl_l_ptr:	ds	2	; Block definition bottom left lo-byte.
blk_br_l_ptr:	ds	2	; Block definition bottom right lo-byte.
blk_tl_h_ptr:	ds	2	; Block definition top left hi-byte.
blk_tr_h_ptr:	ds	2	; Block definition top right hi-byte.
blk_bl_h_ptr:	ds	2	; Block definition bottom left hi-byte.
blk_br_h_ptr:	ds	2	; Block definition bottom right hi-byte.
		.code

	.else

	; If the block definititions are 2KByte aligned.

BLK_4000_TL_L	=	$4000
BLK_4000_TR_L	=	$4100
BLK_4000_BL_L	=	$4200
BLK_4000_BR_L	=	$4300
BLK_4000_TL_H	=	$4400
BLK_4000_TR_H	=	$4500
BLK_4000_BL_H	=	$4600
BLK_4000_BR_H	=	$4700

BLK_4800_TL_L	=	$4800
BLK_4800_TR_L	=	$4900
BLK_4800_BL_L	=	$4A00
BLK_4800_BR_L	=	$4B00
BLK_4800_TL_H	=	$4C00
BLK_4800_TR_H	=	$4D00
BLK_4800_BL_H	=	$4E00
BLK_4800_BR_H	=	$4F00

BLK_5000_TL_L	=	$5000
BLK_5000_TR_L	=	$5100
BLK_5000_BL_L	=	$5200
BLK_5000_BR_L	=	$5300
BLK_5000_TL_H	=	$5400
BLK_5000_TR_H	=	$5500
BLK_5000_BL_H	=	$5600
BLK_5000_BR_H	=	$5700

BLK_5800_TL_L	=	$5800
BLK_5800_TR_L	=	$5900
BLK_5800_BL_L	=	$5A00
BLK_5800_BR_L	=	$5B00
BLK_5800_TL_H	=	$5C00
BLK_5800_TR_H	=	$5D00
BLK_5800_BL_H	=	$5E00
BLK_5800_BR_H	=	$5F00

	.endif

MAP_UNALIGNED_X	=	$80
MAP_UNALIGNED_Y	=	$40

;
;
;

		.bss

; **************
; 8-byte (or 2-byte) entry for each SCREEN in the MULTI_MAP.

		.rsset	0
	.if	BLKDEF_POINTERS
SCR_MAP_PAGE	.rs	1	; 256-byte aligned.
SCR_MAP_BANK	.rs	1
	.else
SCR_MAP_PAGE	.rs	1	; 256-byte aligned.
SCR_MAP_BANK	.rs	1
SCR_BLK_PAGE	.rs	1	; >$4000, >$4800, >$5000, or >$5800.
SCR_BLK_BANK	.rs	1
SCR_TBL_PAGE	.rs	1	; 256-byte aligned.
SCR_TBL_BANK	.rs	1
SCR_CHR_12	.rs	1	; Which CHR banks are used by the BLK, with
SCR_CHR_34	.rs	1	; a max of 16 CHR banks per MULTI_MAP.
	.endif

		; A simple macro to help build a multi-screen map.

	.ifndef	SCREEN
SCREEN		.macro
		db	(((\1) & $1FFF) | $6000) >> 8, bank(\1)
	.if	!BLKDEF_POINTERS
		db	(((\2) & $1FFF) | $4000) >> 8, bank(\2)
		db	(((\3) & $1FFF) | $6000) >> 8, bank(\3)
		dw	(\4)
	.endif
		.endm
	.endif

map_blk_flag:	ds	1	; Returned from _get_map_block().
map_blk_mask:	ds	1	; Returned from _get_map_block().

; **************
; 16-bytes of VDC blkmap info.
;
; N.B. MUST be 16-bytes before the SGX versions to use PCE_VDC_OFFSET.

vdc_old_chr_x:	ds	1	; Previous top-left X in CHR (lo-byte only).
vdc_old_chr_y:	ds	1	; Previous top-left Y in CHR (lo-byte only).

vdc_tbl_addr:	ds	2	; 256-byte aligned.
vdc_tbl_bank:	ds	1

vdc_blk_addr:	ds	2	; 2KBytes of data, 256-byte aligned.
vdc_blk_bank:	ds	1

vdc_map_addr:	ds	2	; Mapped into MPR3..MPR5, max 16KBytes.
vdc_map_bank:	ds	1

vdc_scr_addr:	ds	2	; 8KByte maximum size.
vdc_scr_bank:	ds	1
vdc_scr_chr12:	ds	1	; Which CHR banks are used by the BLK, with
vdc_scr_chr34:	ds	1	; a max of 16 banks per multi-screen map.

	.if	SUPPORT_SGX

; **************
; 16-bytes of SGX blkmap info.
;
; N.B. MUST be 16-bytes after the VDC versions to use SGX_VDC_OFFSET.

sgx_old_chr_x:	ds	1	; Previous top-left X in CHR (lo-byte only).
sgx_old_chr_y:	ds	1	; Previous top-left Y in CHR (lo-byte only).

sgx_tbl_addr:	ds	2	; 256-byte aligned.
sgx_tbl_bank:	ds	1

sgx_blk_addr:	ds	2	; 2KBytes of data, 256-byte aligned.
sgx_blk_bank:	ds	1

sgx_map_addr:	ds	2	; Mapped into MPR3..MPR4, max 8KBytes.
sgx_map_bank:	ds	1

sgx_scr_addr:	ds	2	; 8KByte maximum size.
sgx_scr_bank:	ds	1
sgx_scr_chr12:	ds	1	; Which CHR banks are used by the BLK, with
sgx_scr_chr34:	ds	1	; a max of 16 banks per multi-screen map.

	.endif	SUPPORT_SGX

	.if	0

; **************
; 16-bytes of VDC blkmap info (moved to vdc.asm to save .bss memory).
;
; N.B. MUST be 16-bytes before the SGX versions to use PCE_VDC_OFFSET.

vdc_map_draw_w:	ds	1	; (SCR_WIDTH / 8) + 1
vdc_map_draw_h:	ds	1	; (SCR_HEIGHT / 8) + 1
vdc_map_line_w:	ds	1	; Width of map (or multi-screen BAT) in tiles.
vdc_map_scrn_w:	ds	1	; Width of map in screens (if multi-screen).
vdc_map_pxl_x:	ds	2	; Top-left X of drawn map in pixels.
vdc_map_pxl_y:	ds	2	; Top-left Y of drawn map in pixels.
vdc_map_option:	ds	1	; Flags to disable BAT alignment.

	.if	SUPPORT_SGX
		ds	7	; UNUSED, needed for padding.

; **************
; 16-bytes of SGX blkmap info (moved to vdc.asm to save .bss memory).
;
; N.B. MUST be 16-bytes after the VDC versions to use SGX_VDC_OFFSET.

sgx_map_draw_w:	ds	1	; (SCR_WIDTH / 8) + 1
sgx_map_draw_h:	ds	1	; (SCR_HEIGHT / 8) + 1
sgx_map_line_w:	ds	1	; Width of map (or multi-screen BAT) in tiles.
sgx_map_scrn_w:	ds	1	; Width of map in screens (if multi-screen).
sgx_map_pxl_x:	ds	2	; Top-left X of drawn map in pixels.
sgx_map_pxl_y:	ds	2	; Top-left Y of drawn map in pixels.
sgx_map_option:	ds	1	; Flags to disable BAT alignment.

	.endif	SUPPORT_SGX

	.endif	0

; **************
; Aliases for the asm variable names in HuCC.
;

	.ifdef	HUCC

_map_blk_flag	.alias	map_blk_flag
_map_blk_mask	.alias	map_blk_mask

_vdc_map_draw_w	.alias	vdc_map_draw_w
_vdc_map_draw_h	.alias	vdc_map_draw_h
_vdc_map_pxl_x	.alias	vdc_map_pxl_x
_vdc_map_pxl_y	.alias	vdc_map_pxl_y
_vdc_old_chr_x	.alias	vdc_old_chr_x
_vdc_old_chr_y	.alias	vdc_old_chr_y
_vdc_tbl_addr	.alias	vdc_tbl_addr
_vdc_tbl_bank	.alias	vdc_tbl_bank
_vdc_blk_addr	.alias	vdc_blk_addr
_vdc_blk_bank	.alias	vdc_blk_bank
_vdc_map_line_w	.alias	vdc_map_line_w
_vdc_map_scrn_w	.alias	vdc_map_scrn_w
_vdc_map_addr	.alias	vdc_map_addr
_vdc_map_bank	.alias	vdc_map_bank
_vdc_scr_addr	.alias	vdc_scr_addr
_vdc_scr_bank	.alias	vdc_scr_bank
_vdc_scr_chr12	.alias	vdc_scr_chr12
_vdc_scr_chr34	.alias	vdc_scr_chr34

	.if	SUPPORT_SGX
_sgx_map_draw_w	.alias	sgx_map_draw_w
_sgx_map_draw_h	.alias	sgx_map_draw_h
_sgx_map_pxl_x	.alias	sgx_map_pxl_x
_sgx_map_pxl_y	.alias	sgx_map_pxl_y
_sgx_old_chr_x	.alias	sgx_old_chr_x
_sgx_old_chr_y	.alias	sgx_old_chr_y
_sgx_tbl_addr	.alias	sgx_tbl_addr
_sgx_tbl_bank	.alias	sgx_tbl_bank
_sgx_blk_addr	.alias	sgx_blk_addr
_sgx_blk_bank	.alias	sgx_blk_bank
_sgx_map_line_w	.alias	sgx_map_line_w
_sgx_map_scrn_w	.alias	sgx_map_scrn_w
_sgx_map_addr	.alias	sgx_map_addr
_sgx_map_bank	.alias	sgx_map_bank
_sgx_scr_addr	.alias	sgx_scr_addr
_sgx_scr_bank	.alias	sgx_scr_bank
_sgx_scr_chr12	.alias	sgx_scr_chr12
_sgx_scr_chr34	.alias	sgx_scr_chr34
	.endif	SUPPORT_SGX

	.endif	HUCC

; **************
; Temporary variables for drawing, using common zero-page locations.
;

map_bat_x	=	_al	; Set by draw_map(), scroll_map() if drawing
map_bat_y	=	_ah	; aligned, or as parameters to blit_map().

map_draw_w	=	_bl	; Set by draw_map(), scroll_map(), but given
map_draw_h	=	_bh	; as parameters to blit_map().

map_pxl_x	=	_cx	; Set by draw_map(), scroll_map() and also by
map_chr_x	=	_cl	; blit__map(), from current vdc_map_pxl_x.
map_scrn_x	=	_ch

map_pxl_y	=	_dx	; Set by draw_map(), scroll_map() and also by
map_chr_y	=	_dl	; blit__map(), from current vdc_map_pxl_y.
map_scrn_y	=	_dh

map_line	=	_si	; Start of map data line being drawn.

map_count	=	__temp + 0
map_drawn	=	__temp + 1

		.code



blkmap_group	.procgroup

; ***************************************************************************
; ***************************************************************************
;
; _set_blocks - Initialize the block definition pointers.
; _sgx_set_blocks - Initialize the block definition pointers.
;
; void __fastcall set_blocks( unsigned char __far *blk_def<vdc_blk_bank:vdc_blk_addr>, unsigned char __far *flg_def<vdc_tbl_bank:vdc_tbl_addr>, unsigned char number_of_blk<_al> );
; void __fastcall sgx_set_blocks( unsigned char __far *blk_def<sgx_blk_bank:sgx_blk_addr>, unsigned char __far *flg_def<sgx_tbl_bank:sgx_tbl_addr>, unsigned char number_of_blk<_al> );
;

	.if	SUPPORT_SGX

		.proc	_sgx_set_blocks.3

		ldx	#SGX_VDC_OFFSET		; Offset to SGX VDC.
		db	$F0			; Turn "clx" into a "beq".

		.ref	_set_blocks.3
		.endp
	.endif

		.proc	_set_blocks.3

		clx				; Offset to PCE VDC.

		lda.h	vdc_tbl_addr, x		; Remap the address to MPR2.
		and	#$1F
		ora	#$40
		sta.h	vdc_tbl_addr, x

		lda.h	vdc_blk_addr, x		; Remap the address to MPR2.
		and	#$1F
		ora	#$40
		sta.h	vdc_blk_addr, x

	.if	BLKDEF_POINTERS
		dec	<_al			; Number of blocks.

!:		lda.l	vdc_blk_addr, x		; Get the BLK address.
		ldy.h	vdc_blk_addr, x		; If the page is zero, then
		bne	!+			; retrieve it from the map.

		tma3				; Set the BLK addr and bank
		pha				; from the 3 bytes *before*
		tma4				; the map data.
		pha
		lda.l	vdc_map_addr, x
		sta.l	<_bp
		lda.h	vdc_map_addr, x
		and.h	#$1F00
		ora.h	#$8000
		dec	a
		sta.h	<_bp
		lda	vdc_map_bank, x
		tma4
		dec	a
		tma3
		ldy	#-1
		lda	[_bp], y
		sta	vdc_blk_bank, x
		dey
		lda	[_bp], y
		and	#$1F
		ora	#$40
		sta.h	vdc_blk_addr, x
		dey
		lda	[_bp], y
		sta.l	vdc_blk_addr, x
		pla
		tam4
		pla
		tam3
		bra	!-

!:		clx				; Initialize the pointers.
.loop:		sta.l	blk_tl_l_ptr, x
		say
		sta.h	blk_tl_l_ptr, x
		say
		sec				; Number of BLK - 1,
		adc	<_al			; so that 256 works!
		bcc	!+
		iny
!:		inx
		inx
		cpx	#8 * 2			; Initialize 8 consecutive
		bcc	.loop			; pointers.
	.endif	BLKDEF_POINTERS

		leave

		.endp



; ***************************************************************************
; ***************************************************************************
;
; _draw_map - Draw the entire screen at the current coordinates.
; _sgx_draw_map - Draw the entire screen at the current coordinates.
;
; void __fastcall draw_map( void );
; void __fastcall sgx_draw_map( void );
;

	.if	SUPPORT_SGX

_sgx_draw_map	.proc

		ldx	#SGX_VDC_OFFSET		; Offset to SGX VDC.
		db	$F0			; Turn "clx" into a "beq".

		.ref	_draw_map
		.endp
	.endif

_draw_map	.proc

		clx				; Offset to PCE VDC.

		tma2				; Preserve MPR2..MPR4.
		pha
		tma3
		pha
		tma4
		pha
	.if	BLKMAP_LARGEMAP
		tma5				; Preserve MPR5.
		pha
	.endif

		jsr	map_pxl_2_chr		; Set up the draw coordinates.

		lda	<map_chr_x		; Reset previous X position.
		sta	vdc_old_chr_x, x

		lda	<map_chr_y		; Reset previous Y position,
		inc	a			; ready to draw multiple rows.
		sta	vdc_old_chr_y, x

		lda	vdc_map_draw_w, x	; Draw the whole screen.
		sta	<map_draw_w
		lda	vdc_map_draw_h, x
		sta	<map_draw_h

		jsr	map_scroll_y		; Draw N row of CHR to the BAT.

	.if	BLKMAP_LARGEMAP
		pla				; Restore MPR5.
		tam5
	.endif
		pla				; Restore MPR2..MPR4.
		tam4
		pla
		tam3
		pla
		tam2

		leave

		.endp



; ***************************************************************************
; ***************************************************************************
;
; _scroll_map - Draw a single row of CHR into the BAT to update the edge.
; _sgx_scroll_map - Draw a single row of CHR into the BAT to update the edge.
;
; void __fastcall scroll_map( void );
; void __fastcall sgx_scroll_map( void );
;

	.if	SUPPORT_SGX

_sgx_scroll_map	.proc

		ldx	#SGX_VDC_OFFSET		; Offset to SGX VDC.
		db	$F0			; Turn "clx" into a "beq".

		.ref	_scroll_map
		.endp
	.endif

_scroll_map	.proc

		clx				; Offset to PCE VDC.

	.if	BLKMAP_TIMETEST
		lda.h	#256			; Set the border color.
		stz.l	VCE_CTA
		sta.h	VCE_CTA
		lda	#%01110001
		sta	VCE_CTW
	.endif

		tma2				; Preserve MPR2..MPR4.
		pha
		tma3
		pha
		tma4
		pha
	.if	BLKMAP_LARGEMAP
		tma5				; Preserve MPR5.
		pha
	.endif

		jsr	map_pxl_2_chr		; Set up the draw coordinates.

	.if	BLKMAP_MULTISCR
		lda	<map_scrn_x		; map_scroll_x can change this!
		pha
	.endif

		lda	vdc_map_draw_h, x	; Draw new LHS or RHS if needed.
		sta	<map_draw_h
;		lda	#1			; map_scroll_x only ever draws a
;		sta	<map_draw_w		; single column.
		jsr	map_scroll_x

		lda	vdc_old_chr_x, x	; Restore map_chr_x which could
		sta	<map_chr_x		; be changed by map_scroll_x.

	.if	BLKMAP_MULTISCR
		pla				; Restore before map_scroll_y.
		sta	<map_scrn_x
	.endif

		lda	vdc_map_draw_w, x	; Draw new TOP or BTM if needed.
		sta	<map_draw_w
		lda	#1
		sta	<map_draw_h
		jsr	map_scroll_y

	.if	BLKMAP_LARGEMAP
		pla				; Restore MPR5.
		tam5
	.endif
		pla				; Restore MPR2..MPR4.
		tam4
		pla
		tam3
		pla
		tam2

	.if	BLKMAP_TIMETEST
		stz	VCE_CTW			; Clear the border color.
	.endif

		leave

		.endp



; ***************************************************************************
; ***************************************************************************
;
; _blit_map - Draw a map rectangle to specific BAT coordinates.
; _sgx_blit_map - Draw a map rectangle to specific BAT coordinates.
;
; void __fastcall blit_map( unsigned char tile_x<map_bat_x>, unsigned char tile_y<map_bat_y>, unsigned char tile_w<map_draw_x>, unsigned char tile_h<map_draw_y> );
; void __fastcall sgx_blit_map( unsigned char tile_x<map_bat_x>, unsigned char tile_y<map_bat_y>, unsigned char tile_w<map_draw_x>, unsigned char tile_h<map_draw_y> );
;
; Normally you'd just use _draw_map() and _scroll_map(), but for those folks
; who really wish to take manual control, you can use this.
;

	.if	SUPPORT_SGX

_sgx_blit_map	.proc

		ldx	#SGX_VDC_OFFSET		; Offset to SGX VDC.
		db	$F0			; Turn "clx" into a "beq".

		.ref	_blit_map
		.endp
	.endif

_blit_map	.proc

		clx				; Offset to PCE VDC.

		lda	vdc_scr_bank, x		; Skip this if a multi-screen
		bne	.exit			; blkmap.

		tma2				; Preserve MPR2..MPR4.
		pha
		tma3
		pha
		tma4
		pha
	.if	BLKMAP_LARGEMAP
		tma5				; Preserve MPR5.
		pha
	.endif

		lda	vdc_map_option, x	; Preserve current map options.
		pha
		ora	#MAP_UNALIGNED_X | MAP_UNALIGNED_Y
		sta	vdc_map_option, x

		jsr	map_pxl_2_chr		; Set up the draw coordinates.

		lda	<map_draw_w		; Are we drawing just 1 column?
		cmp	#1
		beq	.draw_column

		; Draw N rows.

.draw_rows:	lda	<map_chr_x		; Reset previous X position.
		sta	vdc_old_chr_x, x

		lda	<map_chr_y		; Reset previous Y position,
		inc	a			; ready to draw rows upwards.
		sta	vdc_old_chr_y, x

		jsr	map_scroll_y		; Draw a row of CHR to the BAT.

		; Drawing completed.

.finished:	pla				; Restore previous map options.
		sta	vdc_map_option, x

	.if	BLKMAP_LARGEMAP
		pla				; Restore MPR5.
		tam5
	.endif
		pla				; Restore MPR2..MPR4.
		tam4
		pla
		tam3
		pla
		tam2

.exit:		leave

		; Draw 1 column.

.draw_column:	lda	<map_chr_x		; Reset previous X position.
		inc	a			; ready to draw 1 column.
		sta	vdc_old_chr_x, x

		lda	<map_chr_y		; Reset previous Y position.
		sta	vdc_old_chr_y, x

		jsr	map_scroll_x		; Draw a single column of CHR.

		bra	.finished

		.endp



; ***************************************************************************
; ***************************************************************************
;
; _get_map_block - Get the block number used at a map pixel coordinate.
; _sgx_get_map_block - Get the block number used at a map pixel coordinate.
;
; unsigned char __fastcall _get_map_block( unsigned int x<map_pxl_x>, unsigned int y<map_pxl_y> );
; unsigned char __fastcall _sgx_get_map_block( unsigned int x<map_pxl_x>, unsigned int y<map_pxl_y> );
;

	.if	SUPPORT_SGX

		.proc	_sgx_get_map_block.2

		ldx	#SGX_VDC_OFFSET		; Offset to SGX VDC.
		db	$F0			; Turn "clx" into a "beq".

		.ref	_get_map_block.2
		.endp
	.endif

		.proc	_get_map_block.2

		clx				; Offset to PCE VDC.

		tma2				; Preserve MPR2..MPR4.
		pha
		tma3
		pha
		tma4
		pha
	.if	BLKMAP_LARGEMAP
		tma5				; Preserve MPR5.
		pha
	.endif

		jsr	xvt_pxl_2_chr		; Set up the query coordinates.

		jsr	map_chr_2_data		; Page in the map address.

		lda	[_bp]			; Read the BLK from the map.
		tay

		lda	<map_chr_x		; Calc the BLK address offset
		lsr	a			; use depending upon even/odd
		lda	<map_chr_y		; CHR coordinates (not BAT to
		and	#1			; support unaligned drawing).
		rol	a
		ora	#4			; Offset to BLK table top byte.
		adc.h	vdc_blk_addr, x		; What is the BLK data address?
		sta.h	<_bp			; $4000, $4800, $5000 or $5800.
		stz.l	<_bp
		lda	[_bp], y		; Read the collision flags from
		and	#%00001100		; the top byte of the CHR.
		lsr	a
		lsr	a
		lsr	a			; Swizzle the bottom bit to get
		bcc	!+			; the same bit order as used in
		ora	#2			; .HALTMAP layer.
!:		sta	map_blk_flag

		lda	vdc_tbl_bank, x		; Is there a table of flags?
		beq	!+
		tam3
;		inc	a
;		tam4
;		lda.l	vdc_tbl_addr, x
;		sta.l	<_bp
		lda.h	vdc_tbl_addr, x		; If so, it must be aligned!
		sta.h	<_bp
		lda	[_bp], y		; Read the .MASKMAP/.OVERMAP
!:		sta	map_blk_mask		; flag value for this BLK.

	.if	BLKMAP_LARGEMAP
		pla				; Restore MPR5.
		tam5
	.endif
		pla				; Restore MPR2..MPR4.
		tam4
		pla
		tam3
		pla
		tam2

		sxy				; Put the BLK number in X.
		cly

		leave				; All done!

		.endp



; ***************************************************************************
; ***************************************************************************
;
; map_pxl_2_chr - Convert PXL to CHR, BLK and SCR coordinates.
;

map_pxl_2_chr:	lda.l	vdc_map_pxl_y, x	; Get current map Y coordinate.
		sta.l	<map_pxl_y
		lda.h	vdc_map_pxl_y, x
		sta.h	<map_pxl_y

		lda.l	vdc_map_pxl_x, x	; Get current map X coordinate.
		sta.l	<map_pxl_x
		lda.h	vdc_map_pxl_x, x
		sta.h	<map_pxl_x

xvt_pxl_2_chr:	lda.h	<map_pxl_x		; Xvert map_pxl_x to map_chr_x.
	.if	BLKMAP_MULTISCR
		tay				; Xvert map_pxl_x to map_scrn_x.
		bit	vdc_bat_width, x
		bvs	.w64
		bpl	.w32
.w128:		lsr	a
.w64:		lsr	a
.w32:		sta	<map_scrn_x
		tya
	.endif
		lsr	a
		ror.l	<map_pxl_x
		lsr	a
		ror.l	<map_pxl_x
		lsr	a
		ror.l	<map_pxl_x		; Max map width is 256 CHR.

		lda.h	<map_pxl_y		; Xvert map_pxl_y to map_chr_y.
	.if	BLKMAP_MULTISCR
		tay				; Xvert map_pxl_y to map_scrn_y.
		bit	vdc_bat_height, x
		bvc	.h32
.h64:		lsr	a
.h32:		sta	<map_scrn_y
		tya
	.endif
		lsr	a
		ror.l	<map_pxl_y
		lsr	a
		ror.l	<map_pxl_y
		lsr	a
		ror.l	<map_pxl_y		; Max map width is 256 CHR.

		rts



; ***************************************************************************
; ***************************************************************************
;
; map_chr_2_data - Page in the MAP address for the CHR coordinates.
;

map_chr_2_data:

	.if	BLKMAP_MULTISCR

		; Initialization for a multi-screen map.

		lda	vdc_scr_bank, x		; Skip this if regular blkmap.
		beq	.regular

.multiscreen:	lda	<map_chr_x		; Compare old_x with cur_x.
		and	vdc_bat_x_mask, x
		sta	<map_bat_x		; Save BAT X chr coordinate.

		lda	<map_chr_y		; Save BAT Y chr coordinate.
		and	vdc_bat_y_mask, x
		sta	<map_bat_y
		lsr	a			; Map BLK Y coordinate.
		sta.h	<map_line
		cla
		bit	vdc_bat_width, x
		bmi	.w128
		bvs	.w64
.w32:		lsr.h	<map_line
		ror	a
.w64:		lsr.h	<map_line
		ror	a
.w128:		lsr.h	<map_line
		ror	a
		lsr.h	<map_line		; Hi-byte of (BLK Y * width).
		ror	a
		sta.l	<map_line		; Lo-byte of (BLK Y * width).

		jsr	map_set_screen		; Put BLK & MAP in MPR2-MPR5.

		lda	<map_bat_x		; Calc map data pointer.
		lsr	a			; Map BLK X coordinate.
		ora.l	<map_line
		sta.l	<_bp
		lda.h	<map_line
		clc
		adc.h	vdc_map_addr, x		; N.B. 256-byte aligned!
		sta.h	<_bp

		rts

	.endif	BLKMAP_MULTISCR

		; Initialization for a regular map.

.regular:	lda	<map_chr_x		; Compare old_x with cur_x.
;		bit	vdc_map_option, x	; Set bit7 to disable aligning
;		bmi	!+			; BAT X with the map X.
		and	vdc_bat_x_mask, x
		sta	<map_bat_x		; Save BAT X chr coordinate.

!:		lda	<map_chr_y		; A = map CHR Y coordinate.
		tay
		lsr	a
		say				; Y = map BLK Y coordinate.

;		bit	vdc_map_option, x	; Set bit6 to disable aligning
;		bvs	!+			; BAT Y with the map Y.
		and	vdc_bat_y_mask, x
		sta	<map_bat_y		; Save BAT Y chr coordinate.

	.if	BLKMAP_LARGEMAP
	.if	FAST_MULTIPLY
!:		lda	#^square_plus_lo	; Put the table-of-squares back
		tma5				; into MPR5.
	.endif
	.endif

!:		lda	vdc_map_line_w, x	; Map width in BLK.
	.if	FAST_MULTIPLY
		sta	<mul_sqrplus_lo		; Takes 54 cycles.
		sta	<mul_sqrplus_hi
		eor	#$FF
		sta	<mul_sqrminus_lo
		sta	<mul_sqrminus_hi
		sec
		lda	[mul_sqrplus_lo], y
		sbc	[mul_sqrminus_lo], y
		sta.l	<map_line		; Lo-byte of (BLK Y * width).
		lda	[mul_sqrplus_hi], y
		sbc	[mul_sqrminus_hi], y
		tay				; Hi-byte of (BLK Y * width).
	.else
		sty.h	<map_line		; Takes 144..176 cycles.
		ldy	#8
		lsr	a
		sta.l	<map_line
		cla
		bcc	.rotate
.add:		clc
		adc.h	<map_line
.rotate:	ror	a
		ror.l	<map_line		; Lo-byte of (BLK Y * width).
		dey
		bcs	.add
		bne	.rotate
		tay				; Hi-byte of (BLK Y * width).
	.endif

!:		lda	<map_chr_x		; Map CHR X coordinate.
		lsr	a			; Map BLK X coordinate.
		clc
		adc.l	<map_line		; Lo-byte of (BLK Y * width).
		bcc	!+
		iny				; Hi-byte of (BLK Y * width).

!:		clc				; Calc map data pointer.
		adc.l	vdc_map_addr, x
		sta.l	<_bp			; Maximum map size is 16KBytes
		tya				; so we don't need to consider
		adc.h	vdc_map_addr, x		; bank overflow.
		sta.h	<_bp

		bra	map_set_banks		; Put BLK & MAP in MPR2-MPR5.



	.if	BLKMAP_MULTISCR

; ***************************************************************************
; ***************************************************************************
;
; map_set_screen -
;
; Maximum X and Y dimension of 128 screens (32768 pixels).
; Maximum 8KByte total size of screen data (1024 screens).
;

map_set_screen:	ldy	<map_scrn_y		; Map SCR Y coordinate.
		lda	vdc_map_scrn_w, x	; Map width in SCREENS.

	.if	FAST_MULTIPLY
		sta	<mul_sqrplus_lo		; Takes 54 cycles.
		sta	<mul_sqrplus_hi
		eor	#$FF
		sta	<mul_sqrminus_lo
		sta	<mul_sqrminus_hi
		sec
		lda	[mul_sqrplus_lo], y
		sbc	[mul_sqrminus_lo], y
		sta.l	<_bp			; Lo-byte of (SCR Y * width).
		lda	[mul_sqrplus_hi], y
		sbc	[mul_sqrminus_hi], y
		tay				; Hi-byte of (SCR Y * width).
	.else
		sty.h	<_bp			; Takes 144..176 cycles.
		ldy	#8
		lsr	a
		sta.l	<_bp
		cla
		bcc	.rotate
.add:		clc
		adc.h	<_bp
.rotate:	ror	a
		ror.l	<_bp			; Lo-byte of (SCR Y * width).
		dey
		bcs	.add
		bne	.rotate
		tay				; Hi-byte of (SCR Y * width).
	.endif

		lda	<map_scrn_x		; Map SCR X coordinate.
		clc
		adc.l	<_bp
		bcc	!+
		iny

!:		sty.h	<_bp			; 2 bytes per screen entry, max
		asl	a			; 8KByte screen table.
		rol.h	<_bp
	.if	BLKDEF_POINTERS == 0
		asl	a			; 8 bytes per screen entry when
		rol.h	<_bp			; not using BLK pointers.
		asl	a
		rol.h	<_bp
	.endif

		adc.l	vdc_scr_addr, x		; Calc screen data pointer.
		sta.l	<_bp			; Maximum data size is 8KBytes
		lda.h	<_bp			; so we don't need to consider
		adc.h	vdc_scr_addr, x		; bank overflow.
		sta.h	<_bp

		lda	vdc_scr_bank, x		; Map the SCR data in MPR3..MPR4.
		tam3
		inc	a
		tam4

		cly
		lda	[_bp], y		; Get SCR_MAP_PAGE.
		sta.h	vdc_map_addr, x
		iny
		lda	[_bp], y		; Get SCR_MAP_BANK.
		sta	vdc_map_bank, x

	.if	BLKDEF_POINTERS == 0
		iny
		lda	[_bp], y		; Get SCR_BLK_PAGE.
		sta.h	vdc_blk_addr, x
		iny
		lda	[_bp], y		; Get SCR_BLK_BANK.
		sta	vdc_blk_bank, x
		iny
		lda	[_bp], y		; Get SCR_TBL_PAGE.
		sta.h	vdc_tbl_addr, x
		iny
		lda	[_bp], y		; Get SCR_TBL_BANK.
		sta	vdc_tbl_bank, x
	.if	0				; These are not currently used.
		iny
		lda	[_bp], y		; Get SCR_CHR_12.
		sta.h	vdc_scr_chr12, x
		iny
		lda	[_bp], y		; Get SCR_CHR_34.
		sta.h	vdc_scr_chr34, x
	.endif
	.endif

;		lda	vdc_bat_width, x	; Set up the map width.
;		lsr	a			; This should have been set
;		sta	vdc_map_line_w, x	; with the multi-screen map.

;		jmp	map_set_banks		; Put BLK & MAP in MPR2-MPR5.

		; Fall through to map_set_banks.

	.endif	BLKMAP_MULTISCR



; ***************************************************************************
; ***************************************************************************
;
; map_set_banks - Put BLK & MAP in MPR2-MPR5.
;

map_set_banks:	lda	vdc_blk_bank, x		; Put the BLK into MPR2.
		tam2

		lda	vdc_map_bank, x		; Put the MAP into MPR3-MPR5.
		tam3
		inc	a
		tam4
	.if	BLKMAP_LARGEMAP
	.if	FAST_MULTIPLY
	.if	BLKMAP_MULTISCR
		ldy	vdc_scr_bank, x		; Multi-screen maps expect
		bne	!+			; table-of-squares in MPR5!
	.endif
	.endif
		inc	a			; Allow for 16KByte blkmap.
		tam5
	.endif

!:		rts



; ***************************************************************************
; ***************************************************************************
;
; map_scroll_x - Update the BAT when X coordinate changes.
;
; N.B. This will alter map_chr_x and map_scrn_x if moved in +ve direction!
;
; N.B. This only ever draws a single column!
;

!no_change:	rts

map_scroll_x:

	.if	BLKMAP_MULTISCR

		; Initialization for a multi-screen map.

		lda	vdc_scr_bank, x		; Skip this if regular blkmap.
		beq	.regular

.multiscreen:	lda	<map_chr_x		; Compare old_x with cur_x.
		cmp	vdc_old_chr_x, x
	.if	BLKMAP_TIMETEST == 0
		beq	!no_change-		; Do nothing if no change.
	.endif
		sta	vdc_old_chr_x, x
		bmi	!+			; Test the sign of the change.

		clc				; Draw RHS if chr_x >= old_x.
		and	vdc_bat_x_mask, x
		adc	vdc_map_draw_w, x	; Usually (SCR_WIDTH / 8) + 1.
		dec	a
		sta	<map_chr_x		; Update CHR X chr coordinate.
		bit	vdc_bat_width, x
		beq	!+
		inc	<map_scrn_x		; Wrapped to the next screen.

!:		and	vdc_bat_x_mask, x
		sta	<map_bat_x		; Save BAT X chr coordinate.

		lda	<map_chr_y		; Save BAT Y chr coordinate.
		and	vdc_bat_y_mask, x
		sta	<map_bat_y
		lsr	a			; Map BLK Y coordinate.
		sta.h	<map_line
		cla
		bit	vdc_bat_width, x
		bmi	.w128
		bvs	.w64
.w32:		lsr.h	<map_line
		ror	a
.w64:		lsr.h	<map_line
		ror	a
.w128:		lsr.h	<map_line
		ror	a
		lsr.h	<map_line		; Hi-byte of (BLK Y * width).
		ror	a
		sta.l	<map_line		; Lo-byte of (BLK Y * width).

		jsr	map_set_screen		; Put BLK & MAP in MPR2-MPR5.

		lda	<map_bat_x		; Calc map data pointer.
		lsr	a			; Map BLK X coordinate.
		ora.l	<map_line
		sta.l	<_bp
		lda.h	<map_line
		clc
		adc.h	vdc_map_addr, x		; N.B. 256-byte aligned!
		sta.h	<_bp

		bra	.draw_col		; Now draw it.

	.endif	BLKMAP_MULTISCR

		; Initialization for a regular map.

.regular:	lda	<map_chr_x		; Compare old_x with cur_x.
		cmp	vdc_old_chr_x, x
	.if	BLKMAP_TIMETEST == 0
		beq	!no_change-		; Do nothing if no change.
	.endif
		sta	vdc_old_chr_x, x
		bmi	!+			; Test the sign of the change.

		clc				; Draw RHS if chr_x >= old_x.
		adc	vdc_map_draw_w, x	; Usually (SCR_WIDTH / 8) + 1.
		dec	a
		sta	<map_chr_x		; Update CHR X chr coordinate.

!:		bit	vdc_map_option, x	; Set bit7 to disable aligning
		bmi	!+			; BAT X with the map X.
		and	vdc_bat_x_mask, x
		sta	<map_bat_x		; Save BAT X chr coordinate.

!:		lda	<map_chr_y		; A = map CHR Y coordinate.
		tay
		lsr	a
		say				; Y = map BLK Y coordinate.

		bit	vdc_map_option, x	; Set bit6 to disable aligning
		bvs	!+			; BAT Y with the map Y.
		and	vdc_bat_y_mask, x
		sta	<map_bat_y		; Save BAT Y chr coordinate.

	.if	BLKMAP_LARGEMAP
	.if	FAST_MULTIPLY
!:		lda	#^square_plus_lo	; Put the table-of-squares back
		tma5				; into MPR5.
	.endif
	.endif

!:		lda	vdc_map_line_w, x	; Map width in BLK.
	.if	FAST_MULTIPLY
		sta	<mul_sqrplus_lo		; Takes 54 cycles.
		sta	<mul_sqrplus_hi
		eor	#$FF
		sta	<mul_sqrminus_lo
		sta	<mul_sqrminus_hi
		sec
		lda	[mul_sqrplus_lo], y
		sbc	[mul_sqrminus_lo], y
		sta.l	<map_line		; Lo-byte of (BLK Y * width).
		lda	[mul_sqrplus_hi], y
		sbc	[mul_sqrminus_hi], y
		tay				; Hi-byte of (BLK Y * width).
	.else
		sty.h	<map_line		; Takes 144..176 cycles.
		ldy	#8
		lsr	a
		sta.l	<map_line
		cla
		bcc	.rotate
.add:		clc
		adc.h	<map_line
.rotate:	ror	a
		ror.l	<map_line		; Lo-byte of (BLK Y * width).
		dey
		bcs	.add
		bne	.rotate
		tay				; Hi-byte of (BLK Y * width).
	.endif

!:		lda	<map_chr_x		; Map CHR X coordinate.
		lsr	a			; Map BLK X coordinate.
		clc
		adc.l	<map_line		; Lo-byte of (BLK Y * width).
		bcc	!+
		iny				; Hi-byte of (BLK Y * width).

!:		clc				; Calc map data pointer.
		adc.l	vdc_map_addr, x
		sta.l	<_bp			; Maximum map size is 16KBytes
		tya				; so we don't need to consider
		adc.h	vdc_map_addr, x		; bank overflow.
		sta.h	<_bp

		jsr	map_set_banks		; Put BLK & MAP in MPR2-MPR5.

		; Draw the first part of the column.

.draw_col:	lda	<map_bat_x		; Set the BAT VRAM destination
		sta.l	<_di			; coordinates.
		lda	<map_bat_y
		sta.h	<_di

		eor	vdc_bat_y_mask, x	; Calc CHR before wrap.
		inc	a
		cmp	vdc_map_draw_h, x	; Usually (SCR_HEIGHT / 8) + 1.
		bcc	!+
		lda	vdc_map_draw_h, x	; Maximum CHR to draw.
!:		sta	<map_count		; Set number of CHR to draw.
		sta	<map_drawn		; Preserve number of CHR drawn.

		lda	#VDC_CR			; Set VDC auto-increment from
		sta	<vdc_reg, x		; the BAT width, which is set
		sta	VDC_AR, x		; up by set_screen_size().
		lda	<vdc_crh, x
		sta	VDC_DH, x

		jsr	blk_col_strip		; Draw top of vertical strip.

		; Wrap around and draw the rest of the column (if needed).

		sec				; Are there any more CHR that
		lda	vdc_map_draw_h, x	; need to be drawn?
		sbc	<map_drawn
		beq	.done

		sta	<map_count		; Set number of CHR to draw.

		lda	<map_chr_y		; Update CHR Y coordinate for
		pha				; drawing unaligned tiles.
		clc
		adc	<map_drawn
		sta	<map_chr_y

	.if	BLKMAP_MULTISCR
		lda	vdc_scr_bank, x		; Skip this if regular blkmap.
		beq	!+

		inc	<map_scrn_y		; Wrapped to the next screen.

		jsr	map_set_screen		; Put BLK & MAP in MPR2-MPR5.

		lda	<map_bat_x		; Calc map data pointer.
		lsr	a			; Map BLK X coordinate.
		sta.l	<_bp
		lda.h	vdc_map_addr, x		; N.B. 256-byte aligned!
		sta.h	<_bp
	.endif

!:		lda	<map_bat_x		; Set the BAT VRAM destination
		sta.l	<_di			; coordinates.
		stz.h	<_di			; Reset 1st row to draw.

		jsr	blk_col_strip		; Draw btm of vertical strip.

	.if	BLKMAP_MULTISCR
		dec	<map_scrn_y		; Restore, no check if should.
	.endif

		pla				; Restore CHR Y coordinate, we
		sta	<map_chr_y		; might draw another column!

.done:		lda	#VDC_CR			; Set VDC auto-increment to 1.
		sta	<vdc_reg, x
		sta	VDC_AR, x
		stz	VDC_DH, x

		rts



; ***************************************************************************
; ***************************************************************************
;
; map_scroll_y - Update the BAT when Y coordinate changes.
;
; N.B. This will alter map_chr_y and map_scrn_y if moved in +ve direction!
;
; N.B. This draws multiple rows when called from _draw_map or _blit_map.
;

!no_change:	rts

map_scroll_y:

	.if	BLKMAP_MULTISCR

		; Initialization for a multi-screen map.

		lda	vdc_scr_bank, x		; Skip this if regular blkmap.
		beq	.regular

.multiscr:	lda	<map_chr_y		; Compare old_y with cur_y.
		cmp	vdc_old_chr_y, x
	.if	BLKMAP_TIMETEST == 0
		beq	!no_change-		; Do nothing if no change.
	.endif
		sta	vdc_old_chr_y, x
		bmi	!+			; Test the sign of the change.

		clc				; Draw bottom if chr_y >= old_y.
		and	vdc_bat_y_mask, x
		adc	vdc_map_draw_h, x	; Usually (SCR_HEIGHT / 8) + 1.
		dec	a
		sta	<map_chr_y		; Update CHR Y chr coordinate.
		bit	vdc_bat_height, x
		beq	!+
		inc	<map_scrn_y		; Wrapped to the next screen.

!:		and	vdc_bat_y_mask, x
		sta	<map_bat_y		; Save BAT Y chr coordinate.

		lsr	a			; Map BLK Y coordinate.
		sta.h	<map_line
		cla
		bit	vdc_bat_width, x
		bmi	.w128
		bvs	.w64
.w32:		lsr.h	<map_line
		ror	a
.w64:		lsr.h	<map_line
		ror	a
.w128:		lsr.h	<map_line
		ror	a
		lsr.h	<map_line		; Hi-byte of (BLK Y * width).
		ror	a
		sta.l	<map_line		; Lo-byte of (BLK Y * width).

		lda	<map_chr_x
		and	vdc_bat_x_mask, x
		sta	<map_bat_x		; Save BAT X chr coordinate.

		; Loop to here if drawing multiple multi-screen rows.

.multiscr_row:	jsr	map_set_screen		; Put BLK & MAP in MPR2-MPR5.

		lda	<map_bat_x		; Calc map data pointer.
		lsr	a			; Map BLK X coordinate.
		ora.l	<map_line
		sta.l	<_bp
		lda.h	<map_line
		clc
		adc.h	vdc_map_addr, x		; N.B. 256-byte aligned!
		sta.h	<_bp

		bra	.draw_row		; Now draw it.

	.endif	BLKMAP_MULTISCR

		; Initialization for a regular blkmap.

.regular:	lda	<map_chr_y		; Compare old_y with cur_y.
		cmp	vdc_old_chr_y, x
	.if	BLKMAP_TIMETEST == 0
		beq	!no_change-		; Do nothing if no change.
	.endif
		sta	vdc_old_chr_y, x
		bmi	!+			; Test the sign of the change.

		clc				; Draw bottom if chr_y >= old_y.
		adc	vdc_map_draw_h, x	; Usually (SCR_HEIGHT / 8) + 1.
		dec	a
		sta	<map_chr_y		; Update CHR Y chr coordinate.

!:		tay				; A = map CHR Y coordinate.
		lsr	a
		say				; Y = map BLK Y coordinate.

		bit	vdc_map_option, x	; Set bit6 to disable aligning
		bvs	!+			; BAT Y with the map Y.
		and	vdc_bat_y_mask, x
		sta	<map_bat_y		; Save BAT Y chr coordinate.

!:		bit	vdc_map_option, x	; Set bit7 to disable aligning
		bmi	!+			; BAT X with the map X.
		lda	<map_chr_x
		and	vdc_bat_x_mask, x
		sta	<map_bat_x		; Save BAT X chr coordinate.
!:

	.if	BLKMAP_LARGEMAP
	.if	FAST_MULTIPLY
		lda	#^square_plus_lo	; Put the table-of-squares back
		tma5				; into MPR5.
	.endif
	.endif

		lda	vdc_map_line_w, x	; Map width in BLK.
	.if	FAST_MULTIPLY
		sta	<mul_sqrplus_lo		; Takes 54 cycles.
		sta	<mul_sqrplus_hi
		eor	#$FF
		sta	<mul_sqrminus_lo
		sta	<mul_sqrminus_hi
		sec
		lda	[mul_sqrplus_lo], y
		sbc	[mul_sqrminus_lo], y
		sta.l	<map_line		; Lo-byte of (BLK Y * width).
		lda	[mul_sqrplus_hi], y
		sbc	[mul_sqrminus_hi], y
		sta.h	<map_line		; Hi-byte of (BLK Y * width).
	.else
		sty.h	<map_line		; Takes 144..176 cycles.
		ldy	#8
		lsr	a
		sta.l	<map_line
		cla
		bcc	.rotate
.add:		clc
		adc.h	<map_line
.rotate:	ror	a
		ror.l	<map_line		; Lo-byte of (BLK Y * width).
		dey
		bcs	.add
		bne	.rotate
		sta.h	<map_line		; Hi-byte of (BLK Y * width).
	.endif

		jsr	map_set_banks		; Put BLK & MAP in MPR2-MPR5.

		; Loop to here if drawing multiple regular blkmap rows.

.regular_row:	ldy.h	<map_line		; Hi-byte of (BLK Y * width).
		lda	<map_chr_x		; Map CHR X coordinate.
		lsr	a			; Map BLK X coordinate.
		clc
		adc.l	<map_line		; Lo-byte of (BLK Y * width).
		bcc	!+
		iny
!:		clc				; Calc map data pointer.
		adc.l	vdc_map_addr, x
		sta.l	<_bp			; Maximum map size is 16KBytes
		tya				; so we don't need to consider
		adc.h	vdc_map_addr, x		; bank overflow.
		sta.h	<_bp

		; Draw the first part of the row.

.draw_row:	lda	<map_bat_y		; Set the BAT VRAM destination
		sta.h	<_di			; coordinates.
		lda	<map_bat_x
		sta.l	<_di

		eor	vdc_bat_x_mask, x	; Calc CHR before wrap.
		inc	a
		cmp	<map_draw_w		; Usually (SCR_WIDTH / 8) + 1.
		bcc	!+
		lda	<map_draw_w		; Maximum CHR to draw.
!:		sta	<map_count		; Set number of CHR to draw.
		sta	<map_drawn		; Preserve number of CHR drawn.

		jsr	blk_row_strip		; Draw lhs of horizontal strip.

		; Wrap around and draw the rest of the row (if needed).

		sec				; Are there any more CHR that
		lda	<map_draw_w		; need to be drawn?
		sbc	<map_drawn
		beq	.done_row

		sta	<map_count		; Set number of CHR to draw.

		lda	<map_chr_x		; Update CHR X coordinate for
		pha				; drawing unaligned tiles.
		clc
		adc	<map_drawn
		sta	<map_chr_x

	.if	BLKMAP_MULTISCR
		lda	vdc_scr_bank, x		; Skip this if regular blkmap.
		beq	!+

		inc	<map_scrn_x		; Wrapped to the next screen.

		jsr	map_set_screen		; Locate screen's BLK and MAP.

		clc				; Calc map data pointer.
		lda.l	<map_line
		sta.l	<_bp
		lda.h	<map_line
		adc.h	vdc_map_addr, x		; N.B. 256-byte aligned!
		sta.h	<_bp
	.endif

!:		lda	<map_bat_y		; Set the BAT VRAM destination
		sta.h	<_di			; coordinates.
		stz.l	<_di			; Reset 1st column to draw.

		jsr	blk_row_strip		; Draw rhs of horizontal strip.

	.if	BLKMAP_MULTISCR
		dec	<map_scrn_x		; Restore, no check if should.
	.endif	BLKMAP_MULTISCR

		pla				; Restore CHR X coordinate, we
		sta	<map_chr_x		; might draw another row!

.done_row:	dec	<map_draw_h		; Are all desired rows drawn?
		beq	.finished

		inc	<map_chr_y		; Move CHR Y down by 1.

		lda	<map_chr_y		; If new BLK then ...
		lsr	a
		bcs	!+
		lda	vdc_map_line_w, x	; Move the map line pointer to
		adc.l	<map_line		; the next line.
		sta.l	<map_line
		bcc	!+
		inc.h	<map_line

!:		lda	<map_bat_y		; Move BAT Y down by 1.
		inc	a
		and	vdc_bat_y_mask, x
		sta	<map_bat_y

	.if	BLKMAP_MULTISCR
		ldy	vdc_scr_bank, x		; Skip this if regular blkmap.
		beq	.regular_row

		tay				; If wrapped to the top line of
		bne	!+			; of the BAT then increment the
		inc	<map_scrn_y		; screen.

		lda	vdc_bat_limit, x	; The map line must wrap around
		lsr	a			; too. This is simple since its
		lsr	a			; size (in bytes) is 1/4 of the
		and.h	<map_line		; BAT size (in words) and it is
		sta.h	<map_line		; a power-of-2.

!:		jmp	.multiscr_row		; Draw next row.
	.else
		bra	.regular_row		; Draw next row.
	.endif	BLKMAP_MULTISCR

.finished:	rts



	.if	BLKDEF_POINTERS

; ***************************************************************************
; ***************************************************************************
;
; blk_row_strip - Draw a single row of CHR into the BAT.
;

blk_row_strip:	jsr	set_di_xy_mawr		; Set the BAT VRAM destination.

		lda	[_bp]			; Get the first BLK number.
		tay

		bbs0	<map_chr_y, .odd_y
.even_y:	bbr0	<map_chr_x, block_top_even
		bra	block_top_odd

.odd_y:		bbr0	<map_chr_x, block_btm_even
		bra	block_btm_odd

		; top horizontal (32+2 rept)
		;
		; 97 cycles per block * 17 -> 1649 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

block_top_even:	lda	[blk_tl_l_ptr], y	; 7
		sta	VDC_DL, x		; 6
		lda	[blk_tl_h_ptr], y	; 7
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

block_top_odd:	lda	[blk_tr_l_ptr], y	; 7
		sta	VDC_DL, x		; 6
		lda	[blk_tr_h_ptr], y	; 7
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; btm horizontal (32+2 rept)
		;
		; 97 cycles per block * 17 -> 1649 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

block_btm_even:	lda	[blk_bl_l_ptr], y	; 7
		sta	VDC_DL, x		; 6
		lda	[blk_bl_h_ptr], y	; 7
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

block_btm_odd:	lda	[blk_br_l_ptr], y	; 7
		sta	VDC_DL, x		; 6
		lda	[blk_br_h_ptr], y	; 7
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts



; ***************************************************************************
; ***************************************************************************
;
; blk_col_strip - Draw a single column of CHR into the BAT.
;

blk_col_strip:	jsr	set_di_xy_mawr		; Set the BAT VRAM destination.

		lda	[_bp]			; Get the first BLK number.
		tay

		bbs0	<map_chr_y, .odd_y
.even_y:	bbr0	<map_chr_x, block_lhs_even
		bra	block_rhs_even

.odd_y:		bbr0	<map_chr_x, block_lhs_odd
		bra	block_rhs_odd

		; lhs vertical (28+2 rept)
		;
		; 108 cycles per block * 15 -> 16200 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

block_lhs_even:	lda	[blk_tl_l_ptr], y	; 7
		sta	VDC_DL, x		; 6
		lda	[blk_tl_h_ptr], y	; 7
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

block_lhs_odd:	lda	[blk_bl_l_ptr], y	; 7
		sta	VDC_DL, x		; 6
		lda	[blk_bl_h_ptr], y	; 7
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		clc				; 2
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; rhs vertical (28+2 rept)
		;
		; 108 cycles per block * 15 -> 1620 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

block_rhs_even:	lda	[blk_tr_l_ptr], y	; 7
		sta	VDC_DL, x		; 6
		lda	[blk_tr_h_ptr], y	; 7
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

block_rhs_odd:	lda	[blk_br_l_ptr], y	; 7
		sta	VDC_DL, x		; 6
		lda	[blk_br_h_ptr], y	; 7
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		clc				; 2
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

	.else	BLKDEF_POINTERS



; ***************************************************************************
; ***************************************************************************
;
; blk_row_strip - Draw a single row of CHR into the BAT.
;

blk_row_strip:	jsr	set_di_xy_mawr		; Set the BAT VRAM destination.

		lda	<map_chr_x		; Select what drawing code to
		lsr	a			; use depending upon even/odd
		lda	<map_chr_y		; CHR coordinates (not BAT to
		and	#1			; support unaligned drawing).
		rol	a
		asl	a
		ora.h	vdc_blk_addr, x		; What is the BLK data address?
		and	#%00011110		; $4000, $4800, $5000 or $5800.
		tay
		lda.h	.jump_table, y		; Push the address of the code.
		pha
		lda.l	.jump_table, y
		pha

		lda	[_bp]			; Get the first BLK number.
		tay

		rts				; Jump to the drawing code.

.jump_table:	dw	b4000_top_even - 1
		dw	b4000_top_odd - 1
		dw	b4000_btm_even - 1
		dw	b4000_btm_odd - 1

		dw	b4800_top_even - 1
		dw	b4800_top_odd - 1
		dw	b4800_btm_even - 1
		dw	b4800_btm_odd - 1

		dw	b5000_top_even - 1
		dw	b5000_top_odd - 1
		dw	b5000_btm_even - 1
		dw	b5000_btm_odd - 1

		dw	b5800_top_even - 1
		dw	b5800_top_odd - 1
		dw	b5800_btm_even - 1
		dw	b5800_btm_odd - 1

		; top horizontal (32+2 rept)
		;
		; 89 cycles per block * 17 -> 1513 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b4000_top_even:	lda	BLK_4000_TL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4000_TL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b4000_top_odd:	lda	BLK_4000_TR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4000_TR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; btm horizontal (32+2 rept)
		;
		; 89 cycles per block * 17 -> 1513 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b4000_btm_even:	lda	BLK_4000_BL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4000_BL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b4000_btm_odd:	lda	BLK_4000_BR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4000_BR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; top horizontal (32+2 rept)
		;
		; 89 cycles per block * 17 -> 1513 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b4800_top_even:	lda	BLK_4800_TL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4800_TL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b4800_top_odd:	lda	BLK_4800_TR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4800_TR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; btm horizontal (32+2 rept)
		;
		; 89 cycles per block * 17 -> 1513 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b4800_btm_even:	lda	BLK_4800_BL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4800_BL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b4800_btm_odd:	lda	BLK_4800_BR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4800_BR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; top horizontal (32+2 rept)
		;
		; 89 cycles per block * 17 -> 1513 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b5000_top_even:	lda	BLK_5000_TL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5000_TL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b5000_top_odd:	lda	BLK_5000_TR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5000_TR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; btm horizontal (32+2 rept)
		;
		; 89 cycles per block * 17 -> 1513 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b5000_btm_even:	lda	BLK_5000_BL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5000_BL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b5000_btm_odd:	lda	BLK_5000_BR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5000_BR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; top horizontal (32+2 rept)
		;
		; 89 cycles per block * 17 -> 1513 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b5800_top_even:	lda	BLK_5800_TL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5800_TL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b5800_top_odd:	lda	BLK_5800_TR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5800_TR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; btm horizontal (32+2 rept)
		;
		; 89 cycles per block * 17 -> 1513 cycles

!repeat:	lda	[_bp]		; 7
		tay				; 2

b5800_btm_even:	lda	BLK_5800_BL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5800_BL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b5800_btm_odd:	lda	BLK_5800_BR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5800_BR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		inc.l	<_bp			; 6
		bne	!+			; 2/4
		inc.h	<_bp			; 6

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts



; ***************************************************************************
; ***************************************************************************
;
; blk_col_strip - Draw a single column of CHR into the BAT.
;

blk_col_strip:	jsr	set_di_xy_mawr		; Set the BAT VRAM destination.

		lda	<map_chr_x		; Select what drawing code to
		lsr	a			; use depending upon even/odd
		lda	<map_chr_y		; CHR coordinates (not BAT to
		and	#1			; support unaligned drawing).
		rol	a
		asl	a			; N.B. Always leaves C clear.
		ora.h	vdc_blk_addr, x		; What is the BLK data address?
		and	#%00011110		; $4000, $4800, $5000 or $5800.
		tay
		lda.h	.jump_table, y		; Push the address of the code.
		pha
		lda.l	.jump_table, y
		pha

		lda	[_bp]			; Get the first BLK number.
		tay

		rts				; Jump to the drawing code.

.jump_table:	dw	b4000_lhs_even - 1
		dw	b4000_rhs_even - 1
		dw	b4000_lhs_odd - 1
		dw	b4000_rhs_odd - 1

		dw	b4800_lhs_even - 1
		dw	b4800_rhs_even - 1
		dw	b4800_lhs_odd - 1
		dw	b4800_rhs_odd - 1

		dw	b5000_lhs_even - 1
		dw	b5000_rhs_even - 1
		dw	b5000_lhs_odd - 1
		dw	b5000_rhs_odd - 1

		dw	b5800_lhs_even - 1
		dw	b5800_rhs_even - 1
		dw	b5800_lhs_odd - 1
		dw	b5800_rhs_odd - 1

		; lhs vertical (28+2 rept)
		;
		; N.B. C is always clear on entry.
		;
		; 100 cycles per block * 15 -> 1500 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b4000_lhs_even:	lda	BLK_4000_TL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4000_TL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b4000_lhs_odd:	lda	BLK_4000_BL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4000_BL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6
		clc				; 2

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; rhs vertical (28+2 rept)
		;
		; N.B. C is always clear on entry.
		;
		; 100 cycles per block * 15 -> 1500 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b4000_rhs_even:	lda	BLK_4000_TR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4000_TR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b4000_rhs_odd:	lda	BLK_4000_BR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4000_BR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6
		clc				; 2

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; lhs vertical (28+2 rept)
		;
		; N.B. C is always clear on entry.
		;
		; 100 cycles per block * 15 -> 1500 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b4800_lhs_even:	lda	BLK_4800_TL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4800_TL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b4800_lhs_odd:	lda	BLK_4800_BL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4800_BL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6
		clc				; 2

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; rhs vertical (28+2 rept)
		;
		; N.B. C is always clear on entry.
		;
		; 100 cycles per block * 15 -> 1500 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b4800_rhs_even:	lda	BLK_4800_TR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4800_TR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b4800_rhs_odd:	lda	BLK_4800_BR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_4800_BR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6
		clc				; 2

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; lhs vertical (28+2 rept)
		;
		; N.B. C is always clear on entry.
		;
		; 100 cycles per block * 15 -> 1500 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b5000_lhs_even:	lda	BLK_5000_TL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5000_TL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b5000_lhs_odd:	lda	BLK_5000_BL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5000_BL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6
		clc				; 2

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; rhs vertical (28+2 rept)
		;
		; N.B. C is always clear on entry.
		;
		; 100 cycles per block * 15 -> 1500 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b5000_rhs_even:	lda	BLK_5000_TR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5000_TR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b5000_rhs_odd:	lda	BLK_5000_BR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5000_BR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6
		clc				; 2

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; lhs vertical (28+2 rept)
		;
		; N.B. C is always clear on entry.
		;
		; 100 cycles per block * 15 -> 1500 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b5800_lhs_even:	lda	BLK_5800_TL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5800_TL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b5800_lhs_odd:	lda	BLK_5800_BL_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5800_BL_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6
		clc				; 2

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

		; rhs vertical (28+2 rept)
		;
		; N.B. C is always clear on entry.
		;
		; 100 cycles per block * 15 -> 1500 cycles

!repeat:	lda	[_bp]			; 7
		tay				; 2

b5800_rhs_even:	lda	BLK_5800_TR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5800_TR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		dec	<map_count		; 6
		beq	!end+			; 2

b5800_rhs_odd:	lda	BLK_5800_BR_L, y	; 5
		sta	VDC_DL, x		; 6
		lda	BLK_5800_BR_H, y	; 5
	.if	BLKDEF_CHR_FLAG
		and	#%11110011		; 2
	.endif
		inc	a			; 2
		sta	VDC_DH, x		; 6

		lda	vdc_map_line_w, x	; 5
		adc.l	<_bp			; 4
		sta.l	<_bp			; 4
		bcc	!+			; 2/4
		inc.h	<_bp			; 6
		clc				; 2

!:		dec	<map_count		; 6
		bne	!repeat-		; 4

!end:		rts

	.endif	BLKDEF_POINTERS

	.endprocgroup	; blkmap_group
