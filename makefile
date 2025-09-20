include config.mk

# Directories to be installed to ROOT.
ROOTDIRS = sbin

DIRS = $(ROOTDIRS) sys

.PHONY: all clean help root $(DIRS)

all: help

clean:
	for d in $(DIRS); do (cd $$d && $(MAKE) clean); done

help:
	@printf "Usage:\n"
	@printf "\t$(MAKE) clean - Clean the whole codebase.\n"
	@printf "\t$(MAKE) root  - Install the root file system to ROOT.\n"

root: $(ROOTDIRS)
	mkdir -p $(ROOT)/sbin
	for d in sbin/*/; do cp -f $$d$$(basename $$d) $(ROOT)/sbin; done

$(DIRS):
	(cd $@ && $(MAKE))
