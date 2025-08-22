include ../../../../config.mk

OUT  = archboot.o
OBJS = mbr.o

ASCMD = $(AS) -o $@
CCCMD = $(CC) $(CFLAGS) -I../include -I../../../include -c -o $@

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

mbr.o:
	$(ASCMD) mbr.s
