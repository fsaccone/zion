include ../config.mk
include config.mk
include objs.mk

OUT = sys.bin

CONFIGH = include/config.h
RAMINIT = raminit.bin

.PHONY: all clean test

all: $(OUT)

clean:
	rm -rf $(OUT) $(CONFIGH) $(RAMINIT) \
	       $(OBJS) $(OBJS:.o=.c.o) $(OBJS:.o=.s.o)
	(cd raminit && make clean)

test: $(OUT)
	exec arch/$(ARCH)/machine/$(MACHINE)/test $(OUT)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -e start -o $@ $^
	$(OBJCOPY) -O binary $@

$(CONFIGH):
	echo '#ifndef _CONFIG_H' >> $@
	echo '#define _CONFIG_H' >> $@

	echo '#define BINARY_SIZE $(BINARY_SIZE)' >> $@

	echo '#endif' >> $@

$(RAMINIT):
	(cd raminit && make $@)
	cp raminit/$@ $@

$(OBJS): $(CONFIGH)
	if [ -f $(@:.o=.c) ]; then \
		$(CC) $(CFLAGS) -Iarch/$(ARCH)/machine/$(MACHINE)/include \
		                -Iarch/$(ARCH)/include \
		                -Iinclude \
		                -c \
		                -o $(@:.o=.c.o) \
		                $(@:.o=.c); \
	fi

	if [ -f $(@:.o=.s) ]; then \
		$(AS) $(ASFLAGS) -o $(@:.o=.s.o) $(@:.o=.s); \
	fi

	$(LD) $(LDFLAGS) -r -o $@ \
	                 $$([ -f $(@:.o=.c.o) ] && printf "$(@:.o=.c.o) ") \
	                 $$([ -f $(@:.o=.s.o) ] && printf "$(@:.o=.s.o) ")
