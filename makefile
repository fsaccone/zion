include config.mk

SYS = sys.bin

.PHONY: all clean \
        sys

all: sys

clean:
	rm -f $(SYS)
	for d in sbin/*/; do (cd $$d && $(MAKE) clean); done
	(cd sys && $(MAKE) clean)

sys: $(SYS)

$(SYS):
	(cd sys && $(MAKE) $@)
	cp sys/$@ $@
