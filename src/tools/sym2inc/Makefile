# Makefile for sym2inc
#
# Written for Linux development version, September 27, 2001 by Dave Shadoff 
#

#
# Defines
#
BASEDIR=..d ..d ..
include ../../Make_src.inc

HDRS =
OBJS = sym2inc.o
EXE = sym2inc$(EXESUFFIX)

all: $(EXE)


$(EXE): $(OBJS) $(LIBS) $(HDRS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	$(CP) $(EXE) $(BINDIR)

#
# Targets
#
include $(MAKETARG)
