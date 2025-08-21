include ../../config.mk

OUT  = libkernel.a
OBJS = log.o usermem.o

CCCMD = $(CC) $(CFLAGS) -I../arch/$(ARCH)/include -I../include -c -o $@

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

log.o:
	$(CCCMD) log.c

usermem.o:
	$(CCCMD) usermem.c
