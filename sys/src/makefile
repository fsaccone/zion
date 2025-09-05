include ../../config.mk

OUT   = sys.o
ENTRY = entry.o
OBJS  = interrupt.o kmain.o mem.o
START = start.o

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(START) $(ENTRY) $(OUT) $(OBJS)

$(OUT): $(START) $(ENTRY) $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

$(ENTRY):
	$(AS) -o $@ ../arch/$(ARCH)/$(@:.o=.s)

$(OBJS):
	$(CC) $(CFLAGS) -I../machine/$(MACHINE)/include \
	                -I../arch/$(ARCH)/include \
	                -I../include -c \
	                -o $@ $(@:.o=.c)

$(START):
	$(AS) -o $@ ../machine/$(MACHINE)/$(@:.o=.s)
