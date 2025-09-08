include ../config.mk

OUT = sys.bin

CONFIGH = include/config.h

START  = machine/$(MACHINE)/start.o

ASTART = arch/$(ARCH)/astart.o

ARCHLIB = arch/$(ARCH)/lib/atomic.o \
          arch/$(ARCH)/lib/core.o \
          arch/$(ARCH)/lib/ctx.o \
          arch/$(ARCH)/lib/interrupt.o \
          arch/$(ARCH)/lib/user.o \
          arch/$(ARCH)/lib/vmem.o

LIB = lib/log.o \
      lib/pagetable.o \
      lib/pmem.o \
      lib/process.o \
      lib/spinlock.o

SRC = src/kmain.o \
      src/interrupt.o \
      src/mem.o

CHECKMACHINE = [ $$(readlink machine/$(MACHINE)/arch) = ../../arch/$(ARCH) ]

NEWCFLAGS = $(CFLAGS) -Imachine/$(MACHINE)/include \
                      -Iarch/$(ARCH)/include \
                      -Iinclude

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) $(CONFIGH) $(START) $(ASTART) $(ARCHLIB) $(LIB) $(SRC)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -machine virt -bios none -kernel $^

$(OUT): $(CONFIGH) $(START) $(ASTART) $(ARCHLIB) $(LIB) $(SRC)
	$(CHECKMACHINE)
	$(LD) $(LDFLAGS) -e start -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(CONFIGH):
	cp config.def.h include/config.h

$(START) $(ASTART):
	$(CHECKMACHINE)
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)

$(ARCHLIB):
	$(CHECKMACHINE)
	if   [ -f $(@:.o=.c) ]; then $(CC) $(NEWCFLAGS) -c -o $@ $(@:.o=.c); \
	elif [ -f $(@:.o=.s) ]; then $(AS) $(ASFLAGS) -o $@ $(@:.o=.s); \
	else echo "No $(@:.o=.c) or $(@:.o=.s) file was found." && exit; fi

$(LIB) $(SRC):
	$(CC) $(NEWCFLAGS) -c -o $@ $(@:.o=.c)
