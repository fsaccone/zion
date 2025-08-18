include ../config.mk

OUT = kernel.img

all: $(OUT)

clean:
	rm -f arch.a sys.a
	(cd arch/$(ARCH)/src && $(MAKE) clean)
	(cd src && $(MAKE) clean)

$(OUT): arch.a sys.a
	$(LD) $(LDFLAGS) -A $(ARCH) -T linker.ld -o $@ $^
	$(OBJCOPY) -O binary $@ $@

arch.a:
	(cd arch/$(ARCH)/src && $(MAKE) $@)
	mv arch/$(ARCH)/src/$@ $@

sys.a:
	(cd src && $(MAKE) $@)
	mv src/$@ $@

test: all
	$(QEMU) $(QEMUFLAGS) -kernel $(OUT)

.PHONY: all clean test
