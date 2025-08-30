include ../config.mk

OUT = sys.bin

ARCHOBJ = archsys.o
COMOBJ  = sys.o
LIB     = libsys.a

CHECKMACHINE = [ $$(readlink machine/$(MACHINE)/arch) = ../../arch/$(ARCH) ]

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) $(ARCHOBJ) $(COMOBJ) $(LIB)
	for d in arch/*/; do \
		(cd $$d/lib && $(MAKE) clean); \
		(cd $$d/src && $(MAKE) clean); \
	done
	(cd lib && $(MAKE) clean)
	(cd src && $(MAKE) clean)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -machine virt -bios none -kernel $^

$(OUT): $(ARCHOBJ) $(COMOBJ) $(LIB)
	$(CHECKMACHINE)
	$(LD) $(LDFLAGS) -T machine/$(MACHINE)/sys.ld -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(ARCHOBJ):
	$(CHECKMACHINE)
	(cd arch/$(ARCH)/src && $(MAKE) $@)
	cp arch/$(ARCH)/src/$@ $@

$(COMOBJ):
	(cd src && $(MAKE) $@)
	cp src/$@ $@

$(LIB):
	(cd lib && $(MAKE) $@)
	cp lib/$@ $@
