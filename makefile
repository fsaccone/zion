include config.mk

SYS = sys.bin

.PHONY: all clean sys

all: sys

sys: $(SYS)

clean:
	rm -f $(SYS)
	(cd sys && $(MAKE) clean)

$(SYS):
	(cd sys && $(MAKE) $@)
	cp sys/$@ $@
