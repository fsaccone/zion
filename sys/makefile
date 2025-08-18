include ../config.mk

OUT   = kernel.img
LDOUT = kernel.tmp

all: $(OUT)

clean:
	rm -f $(OUT) $(LDOUT) boot.o libarch.a libkernel.a src.a
	(cd arch/$(ARCH)/boot && $(MAKE) clean)
	(cd arch/$(ARCH)/lib && $(MAKE) clean)
	(cd lib && $(MAKE) clean)
	(cd src && $(MAKE) clean)

$(OUT): $(LDOUT)
	cp $^ $@
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(LDOUT): boot.o src.a libkernel.a libarch.a
	$(LD) $(LDFLAGS) -A $(ARCH) -T linker.ld -o $@ $^

boot.o:
	(cd arch/$(ARCH)/boot && $(MAKE) $@)
	mv arch/$(ARCH)/boot/$@ $@

libarch.a:
	(cd arch/$(ARCH)/lib && $(MAKE) $@)
	mv arch/$(ARCH)/lib/$@ $@

libkernel.a:
	(cd lib && $(MAKE) $@)
	mv lib/$@ $@

src.a:
	(cd src && $(MAKE) $@)
	mv src/$@ $@

test: $(LDOUT)
	$(QEMU) $(QEMUFLAGS) -kernel $^

.PHONY: all clean test
