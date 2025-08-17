include ../config.mk

OUT = kernel.img

all: $(OUT)

clean:
	rm -f arch.a sys.a
	(cd arch/$(ARCH)/src && $(MAKE) clean)
	(cd src && $(MAKE) clean)

$(OUT): arch.a sys.a
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $^
	$(OBJCOPY) -O binary $@ $@

arch.a:
	(cd arch/$(ARCH)/src && $(MAKE) $@)
	mv arch/$(ARCH)/src/$@ $@

sys.a:
	(cd src && $(MAKE) $@)
	mv src/$@ $@

.PHONY: all clean
