include config.mk

DIRS = sbin sys

.PHONY: all clean \
        sbin sys

all: $(DIRS)

clean:
	for d in $(DIRS); do (cd $$d && $(MAKE) clean); done

$(DIRS):
	(cd $@ && $(MAKE))
