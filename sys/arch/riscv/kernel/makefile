include ../../../../config.mk

OUT  = archkernel.o
OBJS = exception.o interrupt.o trap.o entry.o

ASCMD = $(AS) -o $@
CCCMD = $(CC) $(CFLAGS) -I../include -I../../../include -c -o $@

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

exception.o:
	$(CCCMD) exception.c

interrupt.o:
	$(CCCMD) interrupt.c

entry.o:
	$(ASCMD) entry.s

trap.o:
	$(CCCMD) trap.c
