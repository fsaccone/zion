include ../config.mk

# Out file

OUT = sys.bin

# Object files

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

# Commands

ASCMD      = $(AS) -o $@ $(@:.o=.s) \
             && echo "AS       $@"
CCCMD      = $(CC) $(CFLAGS) -Imachine/$(MACHINE)/include \
                             -Iarch/$(ARCH)/include \
                             -Iinclude \
                             -c -o $@ $(@:.o=.c) \
             && echo "CC       $@"
LDCMD      = $(LD) $(LDFLAGS) -e start -o $@ $^ \
             && echo "LD       $@"
OBJCOPYCMD = $(OBJCOPY) -O binary $@ \
             && echo "OBJCOPY  $@"
STRIPCMD   = $(STRIP) $@ \
             && echo "STRIP    $@"

CHECKMACHINE = [ $$(readlink machine/$(MACHINE)/arch) = ../../arch/$(ARCH) ] \
               || (printf "machine/$(MACHINE)/arch does not point to " \
                && printf "arch/$(ARCH).\n" \
                && exit 1)

# Targets

.PHONY: all clean test

all: $(OUT)

clean:
	@for f in $(OUT) $(START) $(ASTART) $(ARCHLIB) $(LIB) $(SRC); do \
	 if [ -f $$f ]; then rm -f $$f && echo "RM  $$f"; fi; \
	 done

test: $(OUT)
	@printf "\n"
	@$(QEMU) $(QEMUFLAGS) -machine virt -bios none -kernel $^

$(OUT): $(START) $(ASTART) $(ARCHLIB) $(LIB) $(SRC)
	@$(CHECKMACHINE)
	@$(LDCMD)
	@$(STRIPCMD)
	@$(OBJCOPYCMD)

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
