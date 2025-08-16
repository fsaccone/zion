include config.mk

arch.a:
	(cd arch/$(ARCH)/src && $(MAKE) $@)
	mv arch/$(ARCH)/src/$@ $@

sys.a:
	(cd src && $(MAKE) $@)
	mv src/$@ $@
