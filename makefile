include config.mk

SYS = kernel.img

all: sys

sys: $(SYS)

clean:
	rm -f $(SYS)
	(cd sys && $(MAKE) clean)

$(SYS):
	(cd sys && $(MAKE) $@)
	mv sys/$@ $@

.PHONY: all clean sys
