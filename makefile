include config.mk

all: sys

clean:
	rm -f kernel.img
	(cd sys && $(MAKE) clean)

kernel.img:
	(cd sys && $(MAKE) $@)
	mv sys/$@ $@

sys: kernel.img

.PHONY: all clean
