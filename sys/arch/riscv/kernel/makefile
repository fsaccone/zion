include ../../../../config.mk

OUT  = kernel.o
OBJS = ctx.o exception.o interrupt.o trap.o start.o

ASCMD = $(AS) -o $@ $^
CCCMD = $(CC) $(CFLAGS) -I../include -I../../../include -c -o $@ $^

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

ctx.o: ctx.s
	$(ASCMD)

exception.o: exception.c
	$(CCCMD)

interrupt.o: interrupt.c
	$(CCCMD)

start.o: start.s
	$(ASCMD)

trap.o: trap.c
	$(CCCMD)
