include config.mk

# Directories to be installed to ROOTPART.
ROOTDIRS = sbin

DIRS = $(ROOTDIRS) sys

.PHONY: all clean root $(DIRS)

all: root

clean:
	for d in $(DIRS); do (cd $$d && $(MAKE) clean); done

root: $(ROOTDIRS)
	mkdir -p $(ROOTPART)/sbin
	for d in sbin/*/; do cp -f $$d$$(basename $$d) $(ROOTPART)/sbin; done

$(DIRS):
	(cd $@ && $(MAKE))
