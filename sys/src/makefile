include ../../config.mk

OUT    = sys.o

ASTART = astart.o
START  = start.o

OBJS   = interrupt.o kmain.o mem.o

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(ASTART) $(START) $(OUT) $(OBJS)

$(OUT): $(START) $(ASTART) $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

$(ASTART):
	$(AS) -o $@ ../arch/$(ARCH)/$(@:.o=.s)

$(START):
	$(AS) -o $@ ../machine/$(MACHINE)/$(@:.o=.s)

$(OBJS):
	$(CC) $(CFLAGS) -I../machine/$(MACHINE)/include \
	                -I../arch/$(ARCH)/include \
	                -I../include -c \
	                -o $@ $(@:.o=.c)
