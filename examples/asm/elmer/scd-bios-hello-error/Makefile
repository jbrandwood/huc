all: bios-hello-error.iso

include ../Make_ex.inc

AFLAGS ?= --overlay --newproc --strip -m -l 2 -S -gA

SRC_INC = pceas.inc pcengine.inc
SRC_OVL = hello.asm

ERR_INC = pceas.inc pcengine.inc
ERR_OVL = error.asm

SRC_ISO = hello.ovl

bios-hello-error.iso: $(SRC_ISO) error.ovl
	$(IL) bios-hello-error.iso --boot="../include/ipl-scd.dat" --ipl="SuperCD Error" $(SRC_ISO) --cderr error.ovl

hello.ovl: $(SRC_OVL) $(SRC_INC)
	$(AS) $(AFLAGS) --scd hello.asm

# Note that the error overlay is assembled as "--cd" and NOT "--scd"!

error.ovl: $(ERR_OVL) $(ERR_INC)
	$(AS) $(AFLAGS) --cd error.asm
