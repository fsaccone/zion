include ../../../../config.mk

COBJS =
SOBJS = boot.o ptable.o

.PHONY: all clean

all: $(COBJS) $(SOBJS)

clean:
	rm -f $(COBJS) $(SOBJS)

$(COBJS):
	$(CC) $(CFLAGS) -I../include -I../../../include -c -o $@ $(@:.o=.c)

$(SOBJS):
	$(AS) -o $@ $(@:.o=.s)
