include ../config.mk
include objs.mk

OUT = sys.bin

DISK = disk.img

CONFIGH = include/config.h

NEWCFLAGS = $(CFLAGS) -Iarch/$(ARCH)/machine/$(MACHINE)/include \
                      -Iarch/$(ARCH)/include \
                      -Iinclude

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) $(CONFIGH) $(OBJS) $(OBJS:.o=.c.o) $(OBJS:.o=.s.o) $(DISK)

test: $(OUT) $(DISK)
	$(QEMU) $(QEMUFLAGS) -bios none \
	                     -kernel $(OUT) \
	                     -drive id=disk,format=raw,file=$(DISK),if=none \
	                     -device virtio-blk-device,drive=disk,bus=virtio-mmio-bus.0

$(OUT): $(CONFIGH) $(OBJS)
	$(LD) $(LDFLAGS) -e start -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(DISK):
	dd status=none if=/dev/zero of=$@ bs=1M count=100

$(CONFIGH):
	cp config.def.h include/config.h

$(OBJS):
	if [ -f $(@:.o=.c) ]; then \
		$(CC) $(NEWCFLAGS) -c -o $(@:.o=.c.o) $(@:.o=.c); \
	fi

	if [ -f $(@:.o=.s) ]; then \
		$(AS) $(ASFLAGS) -o $(@:.o=.s.o) $(@:.o=.s); \
	fi

	$(LD) $(LDFLAGS) -r -o $@ \
	                 $$([ -f $(@:.o=.c.o) ] && printf "$(@:.o=.c.o) ") \
	                 $$([ -f $(@:.o=.s.o) ] && printf "$(@:.o=.s.o) ")
