include config.mk

kernel.img:
	(cd sys && $(MAKE) $@)
	mv sys/$@ $@
