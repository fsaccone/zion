include ../../config.mk

PROGRAMS = sbin/init

.PHONY: all clean

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)

$(PROGRAMS):
	mkdir -p $$(dirname $@)
	(cd ../../ && $(MAKE) $@/install)
	(cp -f ../../out/$@ $@)

	$(OBJCOPY) -O binary $@
