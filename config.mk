# Target

# To build /sys/, ARCH should match the architecture of MACHINE
MACHINE = virt
ARCH    = riscv64

# Toolchain

CROSS_COMPILE = $(ARCH)-linux-musl-

AR      = $(CROSS_COMPILE)ar
AS      = $(CROSS_COMPILE)as
CC      = $(CROSS_COMPILE)cc
LD      = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

# Toolchain flags

ASFLAGS = $(EXTRAASFLAGS)
CFLAGS  = $(EXTRACFLAGS) -std=c90 -ffreestanding -Wall -Wextra
LDFLAGS = $(EXTRALDFLAGS) -s
