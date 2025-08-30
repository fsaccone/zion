include ../../../../config.mk

OUT   = archsys.o
COBJS = exception.o interrupt.o trap.o
SOBJS = entry.o status.o

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(COBJS) $(SOBJS)

$(OUT): $(COBJS) $(SOBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

$(COBJS):
	$(CC) $(CFLAGS) -I../../../machine/$(MACHINE)/include \
	                -I../include \
	                -I../../../include \
	                -c -o $@ $(@:.o=.c)

$(SOBJS):
	$(AS) -o $@ $(@:.o=.s)
