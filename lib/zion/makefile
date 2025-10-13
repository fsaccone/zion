include ../../config.mk

OUT = libzion.a

COBJS =
SOBJS = arch/$(ARCH)/syscalls/growmem.o \
        arch/$(ARCH)/syscalls/reboot.o \
        arch/$(ARCH)/syscalls/shutdown.o \

.PHONY: all clean install uninstall

all: $(OUT)

clean:
	rm -f $(OUT) $(COBJS) $(SOBJS)

install: $(OUT)
	mkdir -p ../../out/lib
	cp -f $(OUT) ../../out/lib
	chmod 644 ../../out/lib/$(OUT)

	mkdir -p ../../out/include
	cp -rf include/zion ../../out/include/

uninstall:
	rm -rf ../../out/lib/$(OUT) ../../out/include/zion

$(OUT): $(COBJS) $(SOBJS)
	$(AR) -cr $@ $^

$(COBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

$(SOBJS):
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)
