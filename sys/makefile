include ../config.mk

OUT = kernel.bin

ARCHBOOT = boot.o
ARCHLIB  = libarch.a
LIB      = libkernel.a
SRC      = src.a

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) $(ARCHBOOT) $(ARCHLIB) $(LIB) $(SRC)
	(cd arch/$(ARCH)/boot && $(MAKE) clean)
	(cd arch/$(ARCH)/lib && $(MAKE) clean)
	(cd lib && $(MAKE) clean)
	(cd src && $(MAKE) clean)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -kernel $^

$(OUT): $(ARCHBOOT) $(SRC) $(LIB) $(ARCHLIB)
	$(LD) $(LDFLAGS) -A $(ARCH) -T linker.ld -o $@ $^
	$(STRIP) $@

$(ARCHBOOT):
	(cd arch/$(ARCH)/boot && $(MAKE) $@)
	mv arch/$(ARCH)/boot/$@ $@

$(ARCHLIB):
	(cd arch/$(ARCH)/lib && $(MAKE) $@)
	mv arch/$(ARCH)/lib/$@ $@

$(LIB):
	(cd lib && $(MAKE) $@)
	mv lib/$@ $@

$(SRC):
	(cd src && $(MAKE) $@)
	mv src/$@ $@
