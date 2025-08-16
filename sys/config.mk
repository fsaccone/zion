ARCH = riscv

CROSS_COMPILE = $(ARCH)-none-elf-

AR      = ar
AS      = $(CROSS_COMPILE)as
CC      = $(CROSS_COMPILE)cc
LD      = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS  =
LDFLAGS = -s -static
