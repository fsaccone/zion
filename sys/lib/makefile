include ../../config.mk

OUT      = libsys.a
OBJS     = log.o pmem.o process.o spinlock.o
ARCHOBJS = atomic.o core.o ctx.o interrupt.o vmem.o

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
	@if [ -f ../arch/$(ARCH)/lib/$(@:.o=.c) ]; then \
		$(CC) $(CFLAGS) -I../machine/$(MACHINE)/include \
		                -I../arch/$(ARCH)/include \
		                -I../include \
		                -c -o $@ ../arch/$(ARCH)/lib/$(@:.o=.c); \
	elif [ -f ../arch/$(ARCH)/lib/$(@:.o=.s) ]; then \
		$(AS) -o $@ ../arch/$(ARCH)/lib/$(@:.o=.s); \
	else \
		printf "No arch/$(ARCH)/lib/$(@:.o=.c) or "; \
		printf "arch/$(ARCH)/lib/$(@:.o=.s) file.\n"; \
		exit 1; \
	fi
