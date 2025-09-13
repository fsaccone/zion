include ../config.mk

OUT = sys.bin

DISK = disk.img

CONFIGH = include/config.h

START  = arch/$(ARCH)/machine/$(MACHINE)/start.o

ASTART = arch/$(ARCH)/astart.o

ARCHLIB = arch/$(ARCH)/lib/atomic.o \
          arch/$(ARCH)/lib/core.o \
          arch/$(ARCH)/lib/ctx.o \
          arch/$(ARCH)/lib/interrupt.o \
          arch/$(ARCH)/lib/timer.o \
          arch/$(ARCH)/lib/user.o \
          arch/$(ARCH)/lib/vmem.o

LIBDRIVER = lib/driver/uart.o

LIB = lib/log.o \
      lib/pagetable.o \
      lib/pmem.o \
      lib/process.o \
      lib/spinlock.o \
      $(LIBDRIVER)

INTERRUPT = interrupt/interrupt.o \
            interrupt/syscall.o

KERNEL = kernel/drivers.o \
         kernel/kernel.o \
         kernel/mem.o

OBJS = $(OUT) $(CONFIGH) $(START) $(ASTART) $(ARCHLIB) $(LIB) $(INTERRUPT) \
       $(KERNEL)

NEWCFLAGS = $(CFLAGS) -Iarch/$(ARCH)/machine/$(MACHINE)/include \
                      -Iarch/$(ARCH)/include \
                      -Iinclude

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OBJS) $(OBJS:.o=.c.o) $(OBJS:.o=.s.o) $(DISK)

test: $(OUT) $(DISK)
	$(QEMU) $(QEMUFLAGS) -bios none \
	                     -kernel $(OUT) \
	                     -drive id=disk,format=raw,file=$(DISK),if=none \
	                     -device virtio-blk-device,drive=disk,bus=virtio-mmio-bus.0

$(OUT): $(CONFIGH) $(START) $(ASTART) $(ARCHLIB) $(LIB) $(INTERRUPT) $(KERNEL)
	$(LD) $(LDFLAGS) -e start -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(DISK):
	dd status=none if=/dev/zero of=$@ bs=1M count=100

$(CONFIGH):
	cp config.def.h include/config.h

$(START) $(ASTART) $(ARCHLIB) $(LIB) $(INTERRUPT) $(KERNEL):
	if [ -f $(@:.o=.c) ]; then \
		$(CC) $(NEWCFLAGS) -c -o $(@:.o=.c.o) $(@:.o=.c); \
	fi

	if [ -f $(@:.o=.s) ]; then \
		$(AS) $(ASFLAGS) -o $(@:.o=.s.o) $(@:.o=.s); \
	fi

	$(LD) $(LDFLAGS) -r -o $@ \
	                 $$([ -f $(@:.o=.c.o) ] && printf "$(@:.o=.c.o) ") \
	                 $$([ -f $(@:.o=.s.o) ] && printf "$(@:.o=.s.o) ")
