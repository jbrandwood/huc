# Makefile for examples/as/elmer
#

PREREQS = ipl-scd
SUBDIRS = rom-bare-hello rom-bare-buftest rom-bare-dmatest rom-bare-mwrtest rom-bare-rcrtest rom-bare-tiatest \
          rom-core-hello rom-core-hugerom rom-core-okitest \
          scd-bios-hello scd-bios-hello-error \
          cd-core-1stage cd-core-2stage cd-core-scsitest \
          scd-core-1stage scd-core-1stage-error \
          scd-core-2stage scd-core-2stage-error scd-core-fastcd \
          ted2-core-hwdetect ted2-core-sdcard \
          ted2-core-gulliver

all clean:
	@$(MAKE) $(PREREQS) "COMMAND=$@"
	@$(MAKE) $(SUBDIRS) "COMMAND=$@"

.PHONY: $(PREREQS) $(SUBDIRS)

$(PREREQS) $(SUBDIRS):
	@echo " "
	@echo " -----> make $(COMMAND) in $@"
	$(MAKE) --directory=$@ $(COMMAND)
