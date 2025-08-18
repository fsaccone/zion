include ../../config.mk

OUT  = src.a
OBJS = kmain.o

CCCMD = $(CC) $(CFLAGS) -I../arch/$(ARCH)/include -I../include -c -o $@ $^

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

kmain.o: kmain.c
	$(CCCMD)
