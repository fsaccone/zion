include ../config.mk

OUT      = kernel.bin
UNSTROUT = kernel.out

ARCHKERNEL = archkernel.o
ARCHLIB    = libarch.a
LIB        = libkernel.a
KERNEL     = kernel.a

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) $(UNSTROUT) $(ARCHKERNEL) $(ARCHLIB) $(LIB) $(KERNEL)
	(cd arch/$(ARCH)/kernel && $(MAKE) clean)
	(cd arch/$(ARCH)/lib    && $(MAKE) clean)
	(cd lib                 && $(MAKE) clean)
	(cd kernel              && $(MAKE) clean)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -kernel $^

$(OUT): $(UNSTROUT)
	$(STRIP) -o $@ $^
	$(OBJCOPY) -O binary $@

$(UNSTROUT): $(ARCHKERNEL) $(KERNEL) $(LIB) $(ARCHLIB)
	$(LD) $(LDFLAGS) -T arch/$(ARCH)/kernel/linker.ld -o $@ $^

$(ARCHKERNEL):
	(cd arch/$(ARCH)/kernel && $(MAKE) $@)
	cp arch/$(ARCH)/kernel/$@ $@

$(ARCHLIB):
	(cd arch/$(ARCH)/lib && $(MAKE) $@)
	cp arch/$(ARCH)/lib/$@ $@

$(LIB):
	(cd lib && $(MAKE) $@)
	cp lib/$@ $@

$(KERNEL):
	(cd kernel && $(MAKE) $@)
	cp kernel/$@ $@
