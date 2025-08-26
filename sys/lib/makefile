include ../../config.mk

OUT  = libsys.a
OBJS = log.o

ARCHOBJS = boot.o serial.o

CCCMD = $(CC) $(CFLAGS) -I../arch/$(ARCH)/include -I../include -c -o $@

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS) $(ARCHOBJS)

$(OUT): $(OBJS) $(ARCHOBJS)
	$(AR) -cr $@ $^

log.o:
	$(CCCMD) log.c

$(ARCHOBJS):
	(cd ../arch/$(ARCH)/lib && $(MAKE) $@)
	cp ../arch/$(ARCH)/lib/$@ $@
