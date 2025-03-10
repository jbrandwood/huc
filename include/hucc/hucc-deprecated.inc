; ***************************************************************************
; ***************************************************************************
;
; hucc-deprecated.inc
;
; Macros and definitions for compatibility with old HuC/MagicKit projects.
;
; Based on the original HuC and MagicKit definitions by David Michel and the
; other original HuC developers.
;
; ***************************************************************************
; ***************************************************************************

		; For compatibility with HuC projects that use the old name.

HUC_USER_RESERVED = HUC_USER_BANK

		; For compatibility with code that expects the HuC naming that
		; is used in the newer HuC, rather than the original HuC v3.21.
		;
		; Code generated by HuCC uses the original single-underscore.

	.if	1				
__bp		=	$F8:20EC, 2		; base pointer
__si		=	$F8:20EE, 2		; source address
__di		=	$F8:20F0, 2		; destination address

__ax		=	$F8:20F8, 2
__al		=	$F8:20F8, 1
__ah		=	$F8:20F9, 1

__bx		=	$F8:20FA, 2
__bl		=	$F8:20FA, 1
__bh		=	$F8:20FB, 1

__cx		=	$F8:20FC, 2
__cl		=	$F8:20FC, 1
__ch		=	$F8:20FD, 1

__dx		=	$F8:20FE, 2
__dl		=	$F8:20FE, 1
__dh		=	$F8:20FF, 1
	.endif

		; For compatibility with code that expects the HuC naming that
		; is used rather than the shorter upper-case #define naming.

	.if	1				
video_reg	=	$FF:0200, 1		; Address/Status Register
video_reg_l	=	$FF:0200, 1		; Address/Status Register
video_reg_h	=	$FF:0201, 1		; Address/Status Register
video_data	=	$FF:0202, 2		; Data (Read/Write) Low
video_data_l	=	$FF:0202, 1		; Data (Read/Write) Low
video_data_h	=	$FF:0203, 1		; Data (Read/Write) High

color_ctrl	=	$FF:0400, 2		; Control Register
color_reg	=	$FF:0402, 2		; Color Table Address
color_reg_l	=	$FF:0402, 1		; Color Table Address
color_reg_h	=	$FF:0402, 1		; Color Table Address
color_data	=	$FF:0404, 2		; Color Table Data Read/Write
color_data_l	=	$FF:0404, 1		; Color Table Data Read/Write
color_data_h	=	$FF:0405, 1		; Color Table Data Read/Write

psg_ch		=	$FF:0800, 1		; Channel Select
psg_mainvol	=	$FF:0801, 1		; Main Amplitude Level
psg_freqlo	=	$FF:0802, 1		; Frequency Low
psg_freqhi	=	$FF:0803, 1		; Frequency High
psg_ctrl	=	$FF:0804, 1		; Control & Channel Amplitude
psg_pan		=	$FF:0805, 1		; L/R Amplitude Level
psg_wavebuf	=	$FF:0806, 1		; Waveform
psg_noise	=	$FF:0807, 1		; Noise
psg_lfofreq	=	$FF:0808, 1		; LFO Frequency
psg_lfoctrl	=	$FF:0809, 1		; LFO Control

irq_m		=	irq_vec, 1		; Old System Card name.
irq2_jmp	=	irq2_hook, 2		; Old System Card name.
irq1_jmp	=	irq1_hook, 2		; Old System Card name.
timer_jmp	=	timer_hook, 2		; Old System Card name.
nmi_jmp		=	nmi_hook, 2		; Old System Card name.
sat_adr		=	satb_addr, 2		; Old System Card name.
joy		=	joynow, 5		; Old System Card name.
mwr_m		=	vdc_mwr, 1		; Old System Card name.
dcr_m		=	vdc_dcr, 1		; Old System Card name.
	.endif

		; Macros for compatibility with #asm usage in old HuC projects.

stw		.macro
		lda.l	\1
		sta.l	\2
		lda.h	\1
		sta.h	\2
		.endm

stb		.macro
		lda	\1
		sta	\2
		.endm

addw		.macro
	.if	(\# = 3)
		clc
		lda.l	\2
		adc.l	\1
		sta.l	\3
		lda.h	\2
		adc.h	\1
		sta.h	\3
	.else
		clc
		lda.l	\2
		adc.l	\1
		sta.l	\2
		lda.h	\2
		adc.h	\1
		sta.h	\2
	.endif
		.endm

add		.macro			; add byte-sized value to
	.if (\# = 2)			; register A (handle carry
		lda	\2		; flag)
		clc
		adc	\1
		sta	\2
	.else
		clc
		adc	\1
	.endif
		.endm

subw		.macro
		sec
		lda.l	\2
		sbc.l	\1
		sta.l	\2
		lda.h	\2
		sbc.h	\1
		sta.h	\2
		.endm

sub		.macro			; subtract byte-sized value
	.if (\# = 2)			; from register A (handle
		lda	\2		; carry flag)
		sec
		sbc	\1
		sta	\2
	.else
		sec
		sbc	\1
	.endif
		.endm

incw		.macro			; increment a word-sized
		inc.l	\1		; value at stated memory
		bne	!+		; location
		inc.h	\1
!:
		.endm

decw		.macro			; decrement a word-sized
		lda	\1		; value at stated memory
		bne	!+		; location
		dec.h	\1
!:		dec.l	\1
		.endm

lbne		.macro
		bne	\1
		.endm

lbeq		.macro
		beq	\1
		.endm

lbpl		.macro
		bpl	\1
		.endm

lbmi		.macro
		bmi	\1
		.endm

lbcc		.macro
		bcc	\1
		.endm

lbcs		.macro
		bcs	\1
		.endm

lblo		.macro
		bcc	\1
		.endm

lbhs		.macro
		bcs	\1
		.endm

__ldwi		.macro
	__ld.wi		\1
		.endm
