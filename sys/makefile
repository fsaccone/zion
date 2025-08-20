include ../config.mk

OUT = kernel.bin

ARCHKERNEL = kernel.o
ARCHLIB    = libarch.a
LIB        = libkernel.a
SRC        = src.a

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) $(ARCHKERNEL) $(ARCHLIB) $(LIB) $(SRC)
	(cd arch/$(ARCH)/kernel && $(MAKE) clean)
	(cd arch/$(ARCH)/lib && $(MAKE) clean)
	(cd lib && $(MAKE) clean)
	(cd src && $(MAKE) clean)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -kernel $^

$(OUT): $(ARCHKERNEL) $(SRC) $(LIB) $(ARCHLIB)
	$(LD) $(LDFLAGS) -T arch/$(ARCH)/linker.ld -o $@ $^
	$(STRIP) $@

$(ARCHKERNEL):
	(cd arch/$(ARCH)/kernel && $(MAKE) $@)
	mv arch/$(ARCH)/kernel/$@ $@

$(ARCHLIB):
	(cd arch/$(ARCH)/lib && $(MAKE) $@)
	mv arch/$(ARCH)/lib/$@ $@

$(LIB):
	(cd lib && $(MAKE) $@)
	mv lib/$@ $@

$(SRC):
	(cd src && $(MAKE) $@)
	mv src/$@ $@
