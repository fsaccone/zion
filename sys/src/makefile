include ../../config.mk

OUT  = sys.a
OBJS = uart.o kmain.o

CCCMD = $(CC) $(CFLAGS) -I../arch/$(ARCH)/include -c -o $@ $^

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

kmain.o: kmain.c
	$(CCCMD)

uart.o: uart.c
	$(CCCMD)

.PHONY: clean
