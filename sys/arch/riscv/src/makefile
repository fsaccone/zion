include ../../../../config.mk

OUT  = arch.a
OBJS = boot.o uart.o

ASCMD = $(AS) -o $@ $^
CCCMD = $(CC) $(CFLAGS) -I../include -c -o $@ $^

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

boot.o: boot.s
	$(ASCMD)

uart.o: uart.c
	$(CCCMD)
