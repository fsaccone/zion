include ../../config.mk

OUT      = libsys.a
OBJS     = log.o pmem.o
ARCHOBJS = atomic.o boot.o ctx.o interrupt.o vmem.o

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS) $(ARCHOBJS)

$(OUT): $(OBJS) $(ARCHOBJS)
	$(AR) -cr $@ $^

$(OBJS):
	$(CC) $(CFLAGS) -I../machine/$(MACHINE)/include \
	                -I../arch/$(ARCH)/include \
	                -I../include -c \
	                -o $@ $(@:.o=.c)

$(ARCHOBJS):
	(cd ../arch/$(ARCH)/lib && $(MAKE) $@)
	cp ../arch/$(ARCH)/lib/$@ $@
