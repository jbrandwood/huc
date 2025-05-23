# MAGICKIT MAKEFILE
#

#
# Defines
#
BASEDIR=..d ..d ..
include ../../Make_src.inc

OBJS   = main.o input.o assemble.o expr.o code.o command.o\
         macro.o func.o proc.o symbol.o pcx.o output.o crc.o\
         pce.o map.o mml.o nes.o atari.o

LIBS    = pngreadwrite/pngreadwrite.a salvador/salvador.a

CFLAGS  += -Ipngreadwrite -Isalvador

TARGPCE  = pceas$(EXESUFFIX)
TARGNES  = nesasm$(EXESUFFIX)
TARGFUJ  = fujias$(EXESUFFIX)
TARGETS  = $(TARGPCE) $(TARGNES) $(TARGFUJ)

#
#

all::
	$(MAKE) pngreadwrite "COMMAND=$@"
	$(MAKE) salvador "COMMAND=$@"
	$(MAKE) $(TARGETS)

.PHONY: pngreadwrite salvador

pngreadwrite:
	$(MAKE) --directory=$@ $(COMMAND)

salvador:
	$(MAKE) --directory=$@ $(COMMAND)

# ASSEMBLER
#

$(OBJS): defs.h externs.h protos.h
main.o: inst.h vars.h
expr.o: expr.h
pce.o: pce.h
nes.o: nes.h
atari.o: atari.h

# EXE
#

$(TARGPCE) : $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	$(CP) $(TARGPCE) $(BINDIR)

$(TARGNES) : $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	$(CP) $(TARGNES) $(BINDIR)

$(TARGFUJ) : $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	$(CP) $(TARGFUJ) $(BINDIR)

indent:	uncrustify.cfg
	uncrustify -l c -q -c uncrustify.cfg --replace *.c *.h

#
# Targets
#
include $(MAKETARG)

clean::
	$(MAKE) pngreadwrite "COMMAND=$@"
	$(MAKE) salvador "COMMAND=$@"
	rm -f $(TARGETS) $(BINDIR)/$(TARGPCE) $(BINDIR)/$(TARGNES) $(BINDIR)/$(TARGFUJ)
