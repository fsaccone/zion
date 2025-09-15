include config.mk

DIRS = sbin sys

.PHONY: all clean root $(DIRS)

all: root

clean:
	for d in $(DIRS); do \
		(cd $$d && $(MAKE) clean); \
		rm -rf $(ROOTPART)/$$d; \
	done

root: $(ROOTPART)/sbin

$(ROOTPART)/sbin: sbin
	mkdir -p $@
	for d in sbin/*/; do cp -f $$d/$$(basename $$d) $@; done

$(DIRS):
	(cd $@ && $(MAKE))
