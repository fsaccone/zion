OUTS = sbin/init

.PHONY: all clean

all: $(OUTS)

clean:
	rm -f $(OUTS)

$(OUTS):
	mkdir -p $$(dirname $@)
	(cd ../../ && $(MAKE) $@/install)
	(cp -f ../../out/$@ $@)
