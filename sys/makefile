include ../config.mk

OUT = sys.bin

ARCHOBJ = archsys.o
COMOBJ  = sys.o
LIB     = libsys.a

.PHONY: all checkarch clean test

all: $(OUT)

checkarch:
	@[ $$(readlink machine/$(MACHINE)/arch) = ../../arch/$(ARCH) ] \
	 || (echo "ARCH does not match the architecture of MACHINE" >&2 \
	     && exit 1)

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
	$(MAKE) checkarch
	$(LD) $(LDFLAGS) -T machine/$(MACHINE)/sys.ld -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(ARCHOBJ):
	$(MAKE) checkarch
	(cd arch/$(ARCH)/src && $(MAKE) $@)
	cp arch/$(ARCH)/src/$@ $@

$(COMOBJ):
	(cd src && $(MAKE) $@)
	cp src/$@ $@

$(LIB):
	(cd lib && $(MAKE) $@)
	cp lib/$@ $@
