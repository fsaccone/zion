include ../config.mk

OUT = kernel.img

all: $(OUT)

clean:
	rm -f $(OUT) boot.o libarch.a sys.a
	(cd arch/$(ARCH)/boot && $(MAKE) clean)
	(cd arch/$(ARCH)/lib && $(MAKE) clean)
	(cd src && $(MAKE) clean)

$(OUT): boot.o sys.a libarch.a
	$(LD) $(LDFLAGS) -A $(ARCH) -T linker.ld -o $@ $^
	$(OBJCOPY) -O binary $@ $@

boot.o:
	(cd arch/$(ARCH)/boot && $(MAKE) $@)
	mv arch/$(ARCH)/boot/$@ $@

libarch.a:
	(cd arch/$(ARCH)/lib && $(MAKE) $@)
	mv arch/$(ARCH)/lib/$@ $@

sys.a:
	(cd src && $(MAKE) $@)
	mv src/$@ $@

test: all
	$(QEMU) $(QEMUFLAGS) -kernel $(OUT)

.PHONY: all clean test
