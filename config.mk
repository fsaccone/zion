ARCH = riscv

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
QEMUFLAGS = $(EXTRAQEMUFLAGS) -smp 4 -m 512 -serial stdio
