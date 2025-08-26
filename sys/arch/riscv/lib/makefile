include ../../../../config.mk

OUT  = libarch.a
OBJS = boot.o serial.o

ASCMD = $(AS) -o $@
CCCMD = $(CC) $(CFLAGS) -I../include -I../../../include -c -o $@

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -cr $@ $^

boot.o:
	$(ASCMD) boot.s

serial.o:
	$(CCCMD) serial.c
