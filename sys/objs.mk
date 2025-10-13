include ../config.mk

# The first object file to list, in order of linkage, has to be
# arch/ARCH/machine/MACHINE/start.o since it defines the entry point of the
# binary.
OBJS_START = arch/$(ARCH)/machine/$(MACHINE)/start.o

# The second object file is arch/ARCH/astart.o, which defines the function
# start jumps to.
OBJS_ASTART = arch/$(ARCH)/astart.o

# Object files from arch/ARCH/lib/ and lib/.
OBJS_LIB = arch/$(ARCH)/lib/atomic.o \
           arch/$(ARCH)/lib/core.o \
           arch/$(ARCH)/lib/ctx.o \
           arch/$(ARCH)/lib/interrupt.o \
           arch/$(ARCH)/lib/memswitch.o \
           arch/$(ARCH)/lib/timer.o \
           arch/$(ARCH)/lib/trampoline.o \
           lib/console.o \
           lib/exception.o \
           lib/hardware.o \
           lib/pagefault.o \
           lib/panic.o \
           lib/pmem.o \
           lib/process.o \
           lib/schedule.o \
           lib/spinlock.o \
           lib/string.o \
           lib/syscall.o \
           lib/syscall/fork.o \
           lib/syscall/growmem.o \
           lib/syscall/reboot.o \
           lib/syscall/shutdown.o \
           lib/timer.o \
           lib/uart.o \
           lib/vmem.o \

# Object files from src/.
OBJS_SRC = src/kernel.o \
           src/kvmem.o \
           src/mem.o \

# All object files.
OBJS = $(OBJS_START) $(OBJS_ASTART) $(OBJS_LIB) $(OBJS_SRC)
