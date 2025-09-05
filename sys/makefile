include ../config.mk

OUT = sys.bin

LIB = libsys.a
OBJ = sys.o

CHECKMACHINE = [ $$(readlink machine/$(MACHINE)/arch) = ../../arch/$(ARCH) ]

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(LIB) $(OBJ) $(OUT)
	(cd lib && $(MAKE) clean)
	(cd src && $(MAKE) clean)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -machine virt -bios none -kernel $^

$(OUT): $(OBJ) $(LIB)
	$(CHECKMACHINE)
	$(LD) $(LDFLAGS) -e start -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(LIB):
	(cd lib && $(MAKE) $@)
	cp lib/$@ $@

$(OBJ):
	(cd src && $(MAKE) $@)
	cp src/$@ $@
