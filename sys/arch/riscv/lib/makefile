include ../../../../config.mk

OUT  = libarch.a
OBJS = serial.o

CCCMD = $(CC) $(CFLAGS) -I../include -c -o $@

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

serial.o:
	$(CCCMD) serial.c
