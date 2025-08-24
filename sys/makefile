include ../config.mk

OUT = sys.bin

BOOTLOADER_OUT            = bootloader.bin
BOOTLOADER_UNSTRIPPED_OUT = bootloader.out

KERNEL_OUT            = kernel.bin
KERNEL_UNSTRIPPED_OUT = kernel.out

ARCHBOOTLOADER = archbootloader.o
ARCHKERNEL     = archkernel.o
ARCHLIB        = libarch.a

LIB        = libsys.a
BOOTLOADER = bootloader.o
KERNEL     = kernel.o

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) \
	      $(BOOTLOADER_OUT)   $(BOOTLOADER_UNSTRIPPED_OUT) \
	      $(KERNEL_OUT) $(KERNEL_UNSTRIPPED_OUT) \
	      $(ARCHBOOTLOADER) $(ARCHKERNEL) $(ARCHLIB) \
	      $(BOOTLOADER) $(KERNEL) $(LIB)
	for d in arch/*/; do \
		(cd $$d/bootloader && $(MAKE) clean); \
		(cd $$d/kernel     && $(MAKE) clean); \
		(cd $$d/lib        && $(MAKE) clean); \
	done
	(cd bootloader    && $(MAKE) clean)
	(cd kernel        && $(MAKE) clean)
	(cd lib           && $(MAKE) clean)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -no-shutdown -drive file=$^,format=raw

$(OUT): $(BOOTLOADER_OUT) $(KERNEL_OUT)
	cat $^ > $@

	@echo "Padding $(OUT) to make it up to a sector"
	for i in $$(seq $$((512 - $$(stat -c '%s' $(KERNEL_OUT)) % 512))); do \
		dd if=/dev/zero bs=1 count=1 >> $@ 2> /dev/null; \
	done

	chmod +x $@

$(BOOTLOADER_OUT): $(BOOTLOADER_UNSTRIPPED_OUT)
	$(STRIP) -o $@ $^
	$(OBJCOPY) -O binary $@

$(KERNEL_OUT): $(KERNEL_UNSTRIPPED_OUT)
	$(STRIP) -o $@ $^
	$(OBJCOPY) -O binary $@

$(BOOTLOADER_UNSTRIPPED_OUT): $(ARCHBOOTLOADER) $(BOOTLOADER) $(LIB) $(ARCHLIB)
	$(LD) $(LDFLAGS) -T bootloader.ld -o $@ $^

$(KERNEL_UNSTRIPPED_OUT): $(ARCHKERNEL) $(KERNEL) $(LIB) $(ARCHLIB)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

$(ARCHBOOTLOADER):
	(cd arch/$(ARCH)/bootloader && $(MAKE) $@)
	cp arch/$(ARCH)/bootloader/$@ $@

$(ARCHKERNEL):
	(cd arch/$(ARCH)/kernel && $(MAKE) $@)
	cp arch/$(ARCH)/kernel/$@ $@

$(ARCHLIB):
	(cd arch/$(ARCH)/lib && $(MAKE) $@)
	cp arch/$(ARCH)/lib/$@ $@

$(BOOTLOADER):
	(cd bootloader && $(MAKE) $@)
	cp bootloader/$@ $@

$(KERNEL):
	(cd kernel && $(MAKE) $@)
	cp kernel/$@ $@

$(LIB):
	(cd lib && $(MAKE) $@)
	cp lib/$@ $@
