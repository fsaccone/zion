include config.mk

SYS = kernel.img

.PHONY: all clean sys

all: sys

sys: $(SYS)

clean:
	rm -f $(SYS)
	(cd sys && $(MAKE) clean)

$(SYS):
	(cd sys && $(MAKE) $@)
	mv sys/$@ $@
