# To build /sys/, ARCH should match the architecture of MACHINE
MACHINE = virt
ARCH    = riscv64

CROSS_COMPILE = $(ARCH)-linux-musl-

AR      = $(CROSS_COMPILE)ar
AS      = $(CROSS_COMPILE)as
CC      = $(CROSS_COMPILE)cc
LD      = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP   = $(CROSS_COMPILE)strip

ASFLAGS = $(EXTRAASFLAGS)
CFLAGS  = $(EXTRACFLAGS) -ffreestanding -Wall -Wextra
LDFLAGS = $(EXTRALDFLAGS)

QEMU        = qemu-system-$(ARCH)
QEMUCPUS    = 4
QEMUMACHINE = $(MACHINE)
QEMURAM     = 4G
QEMUFLAGS   = $(EXTRAQEMUFLAGS) -machine $(QEMUMACHINE) \
                                -smp $(QEMUCPUS) \
                                -m $(QEMURAM) \
                                -serial stdio
