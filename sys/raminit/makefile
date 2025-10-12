include ../../config.mk

COBJS = raminit.o
SOBJS = arch/$(ARCH)/start.o

OUT = raminit.bin

.PHONY: all clean

all: $(OUT)

clean:
	rm -rf $(OUT) $(COBJS) $(SOBJS)

$(OUT): $(SOBJS) $(COBJS)
	$(LD) $(LDFLAGS) -e start -o $@ $^
	$(OBJCOPY) -O binary $@

$(COBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

$(SOBJS):
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)
