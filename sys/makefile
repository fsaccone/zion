include config.mk

OUT = kernel.img

clean:
	rm -f arch.a sys.a
	(cd arch/$(ARCH)/src && $(MAKE) clean)
	(cd src && $(MAKE) clean)

$(OUT): arch.a sys.a
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@ $@

arch.a:
	(cd arch/$(ARCH)/src && $(MAKE) $@)
	mv arch/$(ARCH)/src/$@ $@

sys.a:
	(cd src && $(MAKE) $@)
	mv src/$@ $@

.PHONY: clean
