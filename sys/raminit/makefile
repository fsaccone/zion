include ../../config.mk

LIBS = libzion.a

COBJS = raminit.o
SOBJS = arch/$(ARCH)/start.o

INCLUDEFLAGS = -I ../../lib/zion/include

OUT = raminit.bin

.PHONY: all clean

all: $(OUT)

clean:
	rm -rf $(OUT) $(COBJS) $(SOBJS) $(LIBS)

$(OUT): $(SOBJS) $(COBJS) $(LIBS)
	$(LD) $(LDFLAGS) -e start -o $@ $^
	$(OBJCOPY) -O binary $@

$(COBJS):
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c -o $@ $(@:.o=.c)

$(SOBJS):
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)

libzion.a:
	(cd ../../lib/zion && $(MAKE) $@ > /dev/null 2>&1)
	cp ../../lib/zion/$@ $@
