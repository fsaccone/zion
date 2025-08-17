include config.mk

all: kernel.img

clean:
	rm -f kernel.img
	(cd sys && $(MAKE) clean)

kernel.img:
	(cd sys && $(MAKE) $@)
	mv sys/$@ $@

.PHONY: all clean
