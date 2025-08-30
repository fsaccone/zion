include ../config.mk

OUT = sys.bin

ARCHKERNEL = archkernel.o
KERNEL     = kernel.o
LIB        = libsys.a

.PHONY: all checkarch clean test

all: $(OUT)

checkarch:
	@[ $$(basename $$(readlink machine/$(MACHINE)/arch)) = $(ARCH) ] \
	 || (echo "ARCH does not match the architecture of MACHINE" >&2 \
	     && exit 1)

clean:
	rm -f $(OUT) $(ARCHKERNEL) $(KERNEL) $(LIB)
	for d in arch/*/; do \
		(cd $$d/lib && $(MAKE) clean); \
		(cd $$d/src && $(MAKE) clean); \
	done
	(cd lib && $(MAKE) clean)
	(cd src && $(MAKE) clean)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -machine virt -bios none -kernel $^

$(OUT): $(ARCHKERNEL) $(KERNEL) $(LIB)
	$(MAKE) checkarch
	$(LD) $(LDFLAGS) -T machine/$(MACHINE)/kernel.ld -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(ARCHKERNEL):
	$(MAKE) checkarch
	(cd arch/$(ARCH)/src && $(MAKE) $@)
	cp arch/$(ARCH)/src/$@ $@

$(KERNEL):
	(cd src && $(MAKE) $@)
	cp src/$@ $@

$(LIB):
	(cd lib && $(MAKE) $@)
	cp lib/$@ $@
