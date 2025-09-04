include ../config.mk

OUT = sys.bin

ENTRY = entry.o
LIB   = libsys.a
OBJ   = sys.o

CHECKMACHINE = [ $$(readlink machine/$(MACHINE)/arch) = ../../arch/$(ARCH) ]

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(ENTRY) $(LIB) $(OBJ) $(OUT)
	(cd lib && $(MAKE) clean)
	(cd src && $(MAKE) clean)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -machine virt -bios none -kernel $^

$(OUT): $(ENTRY) $(LIB) $(OBJ)
	$(CHECKMACHINE)
	$(LD) $(LDFLAGS) -T machine/$(MACHINE)/sys.ld -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(ENTRY):
	$(AS) -o $@ arch/$(ARCH)/$(@:.o=.s)

$(LIB):
	(cd lib && $(MAKE) $@)
	cp lib/$@ $@

$(OBJ):
	(cd src && $(MAKE) $@)
	cp src/$@ $@
