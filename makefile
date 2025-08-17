include config.mk

clean:
	rm -f kernel.img
	(cd sys && $(MAKE) clean)

kernel.img:
	(cd sys && $(MAKE) $@)
	mv sys/$@ $@

.PHONY: clean
