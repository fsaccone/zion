include config.mk

.PHONY: all clean \
        sys

all: sys

clean:
	for d in sbin/*/; do (cd $$d && $(MAKE) clean); done
	(cd sys && $(MAKE) clean)

sys:
	(cd sys && $(MAKE))
