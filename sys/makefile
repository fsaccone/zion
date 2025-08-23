include ../config.mk

BOOT_OUT            = boot.bin
BOOT_UNSTRIPPED_OUT = boot.out

KERNEL_OUT            = kernel.bin
KERNEL_UNSTRIPPED_OUT = kernel.out

ARCHBOOT   = archboot.o
ARCHKERNEL = archkernel.o
ARCHLIB    = libarch.a

LIB        = libsys.a
BOOT       = boot.o
KERNEL     = kernel.o

DISKIMAGE = disk.img

.PHONY: all clean test

all: $(BOOT_OUT) $(KERNEL_OUT)

clean:
	rm -f $(BOOT_OUT)   $(BOOT_UNSTRIPPED_OUT) \
	      $(KERNEL_OUT) $(KERNEL_UNSTRIPPED_OUT) \
	      $(ARCHBOOT) $(ARCHKERNEL) $(ARCHLIB) \
	      $(BOOT) $(KERNEL) $(LIB) \
	      $(DISKIMAGE)
	(cd arch/$(ARCH)/boot   && $(MAKE) clean)
	(cd arch/$(ARCH)/kernel && $(MAKE) clean)
	(cd arch/$(ARCH)/lib    && $(MAKE) clean)
	(cd boot                && $(MAKE) clean)
	(cd kernel              && $(MAKE) clean)
	(cd lib                 && $(MAKE) clean)

test: $(DISKIMAGE)
	$(QEMU) $(QEMUFLAGS) -no-shutdown -drive file=$^,format=raw

$(DISKIMAGE): $(BOOT_OUT) $(KERNEL_OUT)
	cat $^ > $@

	@echo "Padding disk.img to make it up to a sector"
	for i in $$(seq $$((512 - $$(stat -c '%s' $(KERNEL_OUT)) % 512))); do \
		dd if=/dev/zero bs=1 count=1 >> $@ 2> /dev/null; \
	done

	chmod +x $@

$(BOOT_OUT): $(BOOT_UNSTRIPPED_OUT)
	$(STRIP) -o $@ $^
	$(OBJCOPY) -O binary $@

$(KERNEL_OUT): $(KERNEL_UNSTRIPPED_OUT)
	$(STRIP) -o $@ $^
	$(OBJCOPY) -O binary $@

$(BOOT_UNSTRIPPED_OUT): $(ARCHBOOT) $(BOOT) $(LIB) $(ARCHLIB)
	$(LD) $(LDFLAGS) -T arch/$(ARCH)/boot/linker.ld -o $@ $^

$(KERNEL_UNSTRIPPED_OUT): $(ARCHKERNEL) $(KERNEL) $(LIB) $(ARCHLIB)
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
