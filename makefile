include config.mk

DIRS = sbin sys

.PHONY: all clean $(DIRS)

all: $(ROOTPART)/sbin

clean:
	for d in $(DIRS); do \
		(cd $$d && $(MAKE) clean); \
		rm -rf $(ROOTPART)/$$d; \
	done

$(ROOTPART)/sbin: sbin
	mkdir -p $@
	for d in sbin/*/; do cp -f $$d/$$(basename $$d) $@; done

$(DIRS):
	(cd $@ && $(MAKE))
