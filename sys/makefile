include ../config.mk
include objs.mk

OUT = sys.bin

CONFIGH = include/config.h

INIT    = init/
INITTAR = init.tar
INITTARMAGIC = '\x5A\x49\x4F\x4E\x49\x4E\x49\x54\x54\x41\x52\x53\x54\x41\x52\x54'

.PHONY: all clean test

all: $(OUT)

clean:
	rm -rf $(OUT) $(CONFIGH) $(INIT) $(INITTAR) \
	       $(OBJS) $(OBJS:.o=.c.o) $(OBJS:.o=.s.o)

test: $(OUT)
	exec arch/$(ARCH)/machine/$(MACHINE)/test $(OUT)

$(OUT): $(OBJS) $(INITTAR)
	$(LD) $(LDFLAGS) -e start -o $@ $(OBJS)
	$(OBJCOPY) -O binary $@

	dd status=none \
	   if=/dev/zero \
	   bs=1 \
	   count=$$((512 - $$(stat -c '%s' $@) % 512)) >> $@

	printf $(INITTARMAGIC) >> $@
	cat $(INITTAR) >> $@

	dd status=none \
	   if=/dev/zero \
	   bs=1 \
	   count=$$(($(SYS_BINARY_SIZE) \
	             - $$(stat -c '%s' $@) % $(SYS_BINARY_SIZE))) >> $@

$(CONFIGH):
	echo '#ifndef _CONFIG_H' >> $@
	echo '#define _CONFIG_H' >> $@

	echo '#define BINARY_SIZE $(SYS_BINARY_SIZE)' >> $@

	echo '#endif' >> $@

$(INIT):
	mkdir -p $@
	(cd .. && $(MAKE) ROOT=sys/$@ \
	                  SBIN_ENABLED_PROGRAMS='init' \
	                  root)

$(INITTAR): $(INIT)
	(cd $(INIT) && tar -c .) > $@

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
