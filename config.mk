# Target.
ARCH    = riscv64
MACHINE = virt

# Toolchain.
CROSS_COMPILE = $(ARCH)-linux-musl-
AR            = $(CROSS_COMPILE)ar
AS            = $(CROSS_COMPILE)as
CC            = $(CROSS_COMPILE)cc
LD            = $(CROSS_COMPILE)ld
OBJCOPY       = $(CROSS_COMPILE)objcopy

# Toolchain flags.
ASFLAGS = $(EXTRAASFLAGS)
CFLAGS  = $(EXTRACFLAGS) -std=c90 -ffreestanding -Wall -Wextra
LDFLAGS = $(EXTRALDFLAGS) -s

# /sbin/
SBIN_ENABLED_PROGRAMS = init

# /sys/
# 256 K = 256 * 1024 = 262144
SYS_BINARY_SIZE = 262144
