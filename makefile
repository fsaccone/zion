include config.mk

SYS = kernel.img

all: sys

clean:
	rm -f $(SYS)
	(cd sys && $(MAKE) clean)

$(SYS):
	(cd sys && $(MAKE) $@)
	mv sys/$@ $@

sys: $(SYS)

.PHONY: all clean
