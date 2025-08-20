include ../../../../config.mk

OUT  = libarch.a
OBJS = uart.o

CCCMD = $(CC) $(CFLAGS) -I../include -c -o $@ $^

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

uart.o: uart.c
	$(CCCMD)
