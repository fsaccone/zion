include ../config.mk

OUT = sys.bin

BOOTLOADERBIN = bootloader.bin
BOOTLOADEROUT = bootloader.out

KERNELBIN = kernel.bin
KERNELOUT = kernel.out

ARCHBOOTLOADER = archbootloader.o
ARCHKERNEL     = archkernel.o

LIB        = libsys.a
BOOTLOADER = bootloader.o
KERNEL     = kernel.o

.PHONY: all clean test testkernel

all: $(OUT)

clean:
	rm -f $(OUT) \
	      $(BOOTLOADERBIN) $(BOOTLOADEROUT) \
	      $(KERNELBIN) $(KERNELOUT) \
	      $(ARCHBOOTLOADER) $(ARCHKERNEL) \
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

testkernel: $(KERNELBIN)
	$(QEMU) $(QEMUFLAGS) -machine virt -bios none -kernel $^

$(OUT): $(BOOTLOADERBIN) $(KERNELBIN)
	cat $^ > $@
	chmod +x $@

$(BOOTLOADERBIN): $(BOOTLOADEROUT)
	$(STRIP) -o $@ $^
	$(OBJCOPY) -O binary $@

$(KERNELBIN): $(KERNELOUT)
	$(STRIP) -o $@ $^
	$(OBJCOPY) -O binary $@

$(BOOTLOADEROUT): $(ARCHBOOTLOADER) $(BOOTLOADER) $(LIB)
	$(LD) $(LDFLAGS) -T arch/$(ARCH)/bootloader.ld -o $@ $^

$(KERNELOUT): $(ARCHKERNEL) $(KERNEL) $(LIB)
	$(LD) $(LDFLAGS) -T arch/$(ARCH)/kernel.ld -o $@ $^

$(ARCHBOOTLOADER):
	(cd arch/$(ARCH)/bootloader && $(MAKE) $@)
	cp arch/$(ARCH)/bootloader/$@ $@

$(ARCHKERNEL):
	(cd arch/$(ARCH)/kernel && $(MAKE) $@)
	cp arch/$(ARCH)/kernel/$@ $@

$(BOOTLOADER):
	(cd bootloader && $(MAKE) $@)
	cp bootloader/$@ $@

$(KERNEL):
	(cd kernel && $(MAKE) $@)
	cp kernel/$@ $@

$(LIB):
	(cd lib && $(MAKE) $@)
	cp lib/$@ $@
