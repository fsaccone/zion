include ../../config.mk

OUT  = sys.a
OBJS = uart.o kmain.o

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

kmain.o: kmain.c
	$(CC) $(CFLAGS) -I../arch/$(ARCH)/include -c -o $@ $^

uart.o: uart.c
	$(CC) $(CFLAGS) -I../arch/$(ARCH)/include -c -o $@ $^

.PHONY: clean
