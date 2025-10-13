include ../../config.mk

CRT0 = crt0.o
LIBS = libzion.a

OBJS = main.o

INCLUDEFLAGS = -I ../../lib/zion/include

OUT = raminit.bin

.PHONY: all clean

all: $(OUT)

clean:
	rm -rf $(OUT) $(CRT0) $(OBJS) $(LIBS)

$(OUT): $(CRT0) $(OBJS) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@

$(OBJS):
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c -o $@ $(@:.o=.c)

$(CRT0):
	(cd ../../lib/libc && $(MAKE) $@ > /dev/null 2>&1)
	cp ../../lib/libc/$@ $@

libzion.a:
	(cd ../../lib/zion && $(MAKE) $@ > /dev/null 2>&1)
	cp ../../lib/zion/$@ $@
