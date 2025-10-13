include ../../config.mk

OUT = libzion.a

COBJS =
SOBJS = arch/$(ARCH)/syscalls/fork.o \
        arch/$(ARCH)/syscalls/growmem.o \
        arch/$(ARCH)/syscalls/reboot.o \
        arch/$(ARCH)/syscalls/shutdown.o \

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
