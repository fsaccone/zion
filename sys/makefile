include ../config.mk

BOOT_OUT      = boot.bin
BOOT_UNSTROUT = boot.out

KERNEL_OUT      = kernel.bin
KERNEL_UNSTROUT = kernel.out

ARCHBOOT   = archboot.o
ARCHKERNEL = archkernel.o
ARCHLIB    = libarch.a

LIB        = libsys.a
BOOT       = boot.o
KERNEL     = kernel.o

.PHONY: all clean test

all: $(BOOT_OUT) $(KERNEL_OUT)

clean:
	rm -f $(BOOT_OUT)   $(BOOT_UNSTROUT) \
	      $(KERNEL_OUT) $(KERNEL_UNSTROUT) \
	      $(ARCHBOOT) $(ARCHKERNEL) $(ARCHLIB) \
	      $(BOOT) $(KERNEL) $(LIB)
	(cd arch/$(ARCH)/boot   && $(MAKE) clean)
	(cd arch/$(ARCH)/kernel && $(MAKE) clean)
	(cd arch/$(ARCH)/lib    && $(MAKE) clean)
	(cd boot                && $(MAKE) clean)
	(cd kernel              && $(MAKE) clean)
	(cd lib                 && $(MAKE) clean)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -kernel $^

$(BOOT_OUT): $(BOOT_UNSTROUT)
	$(STRIP) -o $@ $^
	$(OBJCOPY) -O binary $@

$(KERNEL_OUT): $(KERNEL_UNSTROUT)
	$(STRIP) -o $@ $^
	$(OBJCOPY) -O binary $@

$(BOOT_UNSTROUT): $(ARCHBOOT) $(BOOT) $(LIB) $(ARCHLIB)
	$(LD) $(LDFLAGS) -T arch/$(ARCH)/boot/linker.ld -o $@ $^

$(KERNEL_UNSTROUT): $(ARCHKERNEL) $(KERNEL) $(LIB) $(ARCHLIB)
	$(LD) $(LDFLAGS) -T arch/$(ARCH)/kernel/linker.ld -o $@ $^

$(ARCHBOOT):
	(cd arch/$(ARCH)/boot && $(MAKE) $@)
	cp arch/$(ARCH)/boot/$@ $@

$(ARCHKERNEL):
	(cd arch/$(ARCH)/kernel && $(MAKE) $@)
	cp arch/$(ARCH)/kernel/$@ $@

$(ARCHLIB):
	(cd arch/$(ARCH)/lib && $(MAKE) $@)
	cp arch/$(ARCH)/lib/$@ $@

$(BOOT):
	(cd boot && $(MAKE) $@)
	cp boot/$@ $@

$(KERNEL):
	(cd kernel && $(MAKE) $@)
	cp kernel/$@ $@

$(LIB):
	(cd lib && $(MAKE) $@)
	cp lib/$@ $@
