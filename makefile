include config.mk

DIRS = sbin sys

.PHONY: all clean \
        sbin sys

all: $(DIRS)

clean:
	for d in sbin/*/; do (cd $$d && $(MAKE) clean); done
	(cd sys && $(MAKE) clean)

$(DIRS):
	(cd $@ && $(MAKE))
