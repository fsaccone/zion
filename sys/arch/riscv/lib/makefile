include ../../../../config.mk

OBJS = boot.o ptable.o

ASCMD = $(AS) -o $@
CCCMD = $(CC) $(CFLAGS) -I../include -I../../../include -c -o $@

.PHONY: all clean

all: $(OBJS)

clean:
	rm -f $(OBJS)

boot.o:
	$(ASCMD) boot.s

ptable.o:
	$(ASCMD) ptable.s
