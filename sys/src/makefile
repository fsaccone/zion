include ../../config.mk

OUT    = sys.o
ASTART = astart.o
OBJS   = interrupt.o kmain.o mem.o
START  = start.o

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(ASTART) $(START) $(OUT) $(OBJS)

$(OUT): $(START) $(ASTART) $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

$(ASTART):
	$(AS) -o $@ ../arch/$(ARCH)/$(@:.o=.s)

$(OBJS):
	$(CC) $(CFLAGS) -I../machine/$(MACHINE)/include \
	                -I../arch/$(ARCH)/include \
	                -I../include -c \
	                -o $@ $(@:.o=.c)

$(START):
	$(AS) -o $@ ../machine/$(MACHINE)/$(@:.o=.s)
