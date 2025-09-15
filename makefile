include config.mk

# Directories to be installed to ROOTPART.
ROOTDIRS = sbin

DIRS = $(ROOTDIRS) sys

.PHONY: all clean help root $(DIRS)

all: help

clean:
	for d in $(DIRS); do (cd $$d && $(MAKE) clean); done

help:
	@printf "Usage:\n"
	@printf "\t$(MAKE) clean - Clean the whole codebase.\n"
	@printf "\t$(MAKE) root  - Install the root file system to ROOTPART.\n"

root: $(ROOTDIRS)
	mkdir -p $(ROOTPART)/sbin
	for d in sbin/*/; do cp -f $$d$$(basename $$d) $(ROOTPART)/sbin; done

$(DIRS):
	(cd $@ && $(MAKE))
