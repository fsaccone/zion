include ../../../../config.mk

OUT  = kernel.o
OBJS = ctx.o exception.o interrupt.o trap.o start.o

ASCMD = $(AS) -o $@
CCCMD = $(CC) $(CFLAGS) -I../include -I../../../include -c -o $@

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

ctx.o:
	$(ASCMD) ctx.s

exception.o:
	$(CCCMD) exception.c

interrupt.o:
	$(CCCMD) interrupt.c

start.o:
	$(ASCMD) start.s

trap.o:
	$(CCCMD) trap.c
