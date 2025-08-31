include ../../../../config.mk

COBJS = vmem.o
SOBJS = boot.o

.PHONY: all clean

all: $(COBJS) $(SOBJS)

clean:
	rm -f $(COBJS) $(SOBJS)

$(COBJS):
	$(CC) $(CFLAGS) -I../../../machine/$(MACHINE)/include \
	                -I../include \
	                -I../../../include \
	                -c -o $@ $(@:.o=.c)

$(SOBJS):
	$(AS) -o $@ $(@:.o=.s)
