# To build /sys/, ARCH should match the architecture of MACHINE
MACHINE = qemu
ARCH    = riscv

CROSS_COMPILE = $(ARCH)-none-elf-

AR      = $(CROSS_COMPILE)ar
AS      = $(CROSS_COMPILE)as
CC      = $(CROSS_COMPILE)cc
LD      = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP   = $(CROSS_COMPILE)strip

CFLAGS  = $(EXTRACFLAGS) -std=c90 -Wall -Wextra -Wpedantic
LDFLAGS = $(EXTRALDFLAGS) -static

QEMU      = qemu-system-$(ARCH)
QEMUCPUS  = 4
QEMURAM   = 128M
QEMUFLAGS = $(EXTRAQEMUFLAGS) -smp $(QEMUCPUS) -m $(QEMURAM) -serial stdio
