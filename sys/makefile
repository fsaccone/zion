include ../config.mk

OUT = sys.bin

START  = machine/$(MACHINE)/start.o

ASTART = arch/$(ARCH)/astart.o

ARCHLIB = arch/$(ARCH)/lib/atomic.o \
          arch/$(ARCH)/lib/core.o \
          arch/$(ARCH)/lib/ctx.o \
          arch/$(ARCH)/lib/interrupt.o \
          arch/$(ARCH)/lib/pagetable.o \
          arch/$(ARCH)/lib/user.o \
          arch/$(ARCH)/lib/vmem.o

LIB     = lib/log.o \
          lib/pmem.o \
          lib/process.o \
          lib/spinlock.o

SRC     = src/kmain.o \
          src/interrupt.o

CHECKMACHINE = [ $$(readlink machine/$(MACHINE)/arch) = ../../arch/$(ARCH) ]

NEWCFLAGS = $(CFLAGS) -Imachine/$(MACHINE)/include \
                      -Iarch/$(ARCH)/include \
                      -Iinclude

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) $(START) $(ASTART) $(ARCHLIB) $(LIB) $(SRC)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -machine virt -bios none -kernel $^

$(OUT): $(START) $(ASTART) $(ARCHLIB) $(LIB) $(SRC)
	$(CHECKMACHINE)
	$(LD) $(LDFLAGS) -e start -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

$(START) $(ASTART):
	$(CHECKMACHINE)
	$(AS) -o $@ $(@:.o=.s)

$(ARCHLIB):
	$(CHECKMACHINE)
	if   [ -f $(@:.o=.c) ]; then $(CC) $(NEWCFLAGS) -c -o $@ $(@:.o=.c); \
	elif [ -f $(@:.o=.s) ]; then $(AS) -o $@ $(@:.o=.s); \
	else echo "No $(@:.o=.c) or $(@:.o=.s) file was found." && exit; fi

$(LIB):
	$(CC) $(NEWCFLAGS) -c -o $@ $(@:.o=.c)

$(SRC):
	$(CC) $(NEWCFLAGS) -c -o $@ $(@:.o=.c)
