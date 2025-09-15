include ../config.mk

# The first object file to list, in order of linkage, has to be
# arch/ARCH/machine/MACHINE/start.o since it defines the entry point of the
# binary.
OBJS_START = arch/$(ARCH)/machine/$(MACHINE)/start.o

# Object files from interrupt/.
OBJS_INTERRUPT_SYSCALL = interrupt/syscall/shutdown.o \
                         interrupt/syscall/reboot.o
OBJS_INTERRUPT         = $(OBJS_INTERRUPT_SYSCALL) \
                         interrupt/interrupt.o \
                         interrupt/syscall.o

# Object files from kernel/ with the addition of arch/ARCH/astart.o.
OBJS_KERNEL = arch/$(ARCH)/astart.o \
              kernel/core.o \
              kernel/drivers.o \
              kernel/kernel.o \
              kernel/mem.o

# Object files from arch/ARCH/lib/ and lib/.
OBJS_ARCH_LIB   = arch/$(ARCH)/lib/atomic.o \
                  arch/$(ARCH)/lib/core.o \
                  arch/$(ARCH)/lib/ctx.o \
                  arch/$(ARCH)/lib/interrupt.o \
                  arch/$(ARCH)/lib/timer.o \
                  arch/$(ARCH)/lib/user.o \
                  arch/$(ARCH)/lib/vmem.o
OBJS_LIB_DRIVER = lib/driver/uart.o
OBJS_LIB        = $(OBJS_ARCH_LIB) \
                  $(OBJS_LIB_DRIVER) \
                  lib/console.o \
                  lib/pagetable.o \
                  lib/panic.o \
                  lib/pmem.o \
                  lib/process.o \
                  lib/spinlock.o

# All object files.
OBJS = $(OBJS_START) $(OBJS_INTERRUPT) $(OBJS_KERNEL) $(OBJS_LIB)
