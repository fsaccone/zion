include ../config.mk

# The first object file to list, in order of linkage, has to be
# arch/ARCH/machine/MACHINE/start.o since it defines the entry point of the
# binary.
OBJS_START = arch/$(ARCH)/machine/$(MACHINE)/start.o

# Object files from interrupt/.
OBJS_INTERRUPT = interrupt/interrupt.o \
                 interrupt/syscall.o \
                 interrupt/syscall/reboot.o \
                 interrupt/syscall/shutdown.o \

# Object files from kernel/ with the addition of arch/ARCH/astart.o.
OBJS_KERNEL = arch/$(ARCH)/astart.o \
              kernel/core.o \
              kernel/init.o \
              kernel/inittar.o \
              kernel/kernel.o \
              kernel/mem.o \

# Object files from arch/ARCH/lib/ and lib/.
OBJS_LIB = arch/$(ARCH)/lib/atomic.o \
           arch/$(ARCH)/lib/core.o \
           arch/$(ARCH)/lib/ctx.o \
           arch/$(ARCH)/lib/interrupt.o \
           arch/$(ARCH)/lib/timer.o \
           arch/$(ARCH)/lib/user.o \
           lib/console.o \
           lib/panic.o \
           lib/pmem.o \
           lib/process.o \
           lib/schedule.o \
           lib/spinlock.o \
           lib/string.o \
           lib/tar.o \
           lib/uart.o \
           lib/vmem.o \

# All object files.
OBJS = $(OBJS_START) $(OBJS_INTERRUPT) $(OBJS_KERNEL) $(OBJS_LIB)
