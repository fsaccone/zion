include ../../../../config.mk

OUT  = libarch.a
OBJS = bss.o serial.o

ASCMD = $(AS) -o $@
CCCMD = $(CC) $(CFLAGS) -I../include -c -o $@

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -cr $@ $^

bss.o:
	$(ASCMD) bss.s

serial.o:
	$(CCCMD) serial.c
