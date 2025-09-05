include ../config.mk

OUT = sys.bin

START  = machine/$(MACHINE)/start.o
ASTART = arch/$(ARCH)/astart.o

ARCHLIB = arch/$(ARCH)/lib/atomic.o \
          arch/$(ARCH)/lib/core.o \
          arch/$(ARCH)/lib/ctx.o \
          arch/$(ARCH)/lib/interrupt.o \
          arch/$(ARCH)/lib/vmem.o
LIB     = lib/log.o \
          lib/pmem.o \
          lib/process.o \
          lib/spinlock.o
SRC     = src/kmain.o \
          src/interrupt.o \
          src/mem.o

ASCMD    = $(AS) -o $@ $(@:.o=.s) \
           && echo "AS     $@"
CCCMD    = $(CC) $(CFLAGS) -Imachine/$(MACHINE)/include \
                           -Iarch/$(ARCH)/include \
                           -Iinclude \
                           -c -o $@ $(@:.o=.c) \
           && echo "CC     $@"
LDCMD    = $(LD) $(LDFLAGS) -e start -o $@ $^ \
           && echo "LD     $@"
STRIPCMD = $(STRIP) $@ \
           && echo "STRIP  $@"

CHECKMACHINE = [ $$(readlink machine/$(MACHINE)/arch) = ../../arch/$(ARCH) ] \
               || (printf "machine/$(MACHINE)/arch does not point to " \
                && printf "arch/$(ARCH).\n" \
                && exit 1)

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) $(START) $(ASTART) $(ARCHLIB) $(LIB) $(SRC)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -machine virt -bios none -kernel $^

$(OUT): $(START) $(ASTART) $(ARCHLIB) $(LIB) $(SRC)
	@$(CHECKMACHINE)
	@$(LDCMD)
	@$(STRIPCMD)
	$(OBJCOPY) -O binary $@

$(START):
	@$(CHECKMACHINE)
	@$(ASCMD)

$(ASTART):
	@$(CHECKMACHINE)
	@$(ASCMD)

$(ARCHLIB):
	@$(CHECKMACHINE)
	@if   [ -f $(@:.o=.c) ]; then $(CCCMD); \
	 elif [ -f $(@:.o=.s) ]; then $(ASCMD); \
	 else echo "No $(@:.o=.c) or $(@:.o=.s) file was found." && exit; fi

$(LIB):
	@$(CCCMD)

$(SRC):
	@$(CCCMD)
