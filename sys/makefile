include config.mk

clean:
	rm -f arch.a sys.a
	(cd arch/$(ARCH)/src && $(MAKE) clean)
	(cd src && $(MAKE) clean)

arch.a:
	(cd arch/$(ARCH)/src && $(MAKE) $@)
	mv arch/$(ARCH)/src/$@ $@

sys.a:
	(cd src && $(MAKE) $@)
	mv src/$@ $@

.PHONY: clean
