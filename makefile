include config.mk

.PHONY: all clean \
        sbin sys

all: sbin sys

clean:
	for d in sbin/*/; do (cd $$d && $(MAKE) clean); done
	(cd sys && $(MAKE) clean)

sbin sys:
	(cd $@ && $(MAKE))
