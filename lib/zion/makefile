include ../../config.mk

OUT = libzion.a

COBJS =
SOBJS = arch/$(ARCH)/syscall/exit.o \
        arch/$(ARCH)/syscall/fork.o \
        arch/$(ARCH)/syscall/growmem.o \
        arch/$(ARCH)/syscall/reboot.o \
        arch/$(ARCH)/syscall/shutdown.o \

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(COBJS) $(SOBJS)

$(OUT): $(COBJS) $(SOBJS)
	$(AR) -cr $@ $^

$(COBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

$(SOBJS):
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)
