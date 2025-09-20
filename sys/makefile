include ../config.mk
include objs.mk

OUT = sys.bin

INIT    = init/
INITTAR = init.tar

.PHONY: all clean test

all: $(OUT)

clean:
	rm -rf $(OUT) $(INIT) $(INITTAR) \
	       $(OBJS) $(OBJS:.o=.c.o) $(OBJS:.o=.s.o)

test: $(OUT)
	exec arch/$(ARCH)/machine/$(MACHINE)/test $(OUT)

$(OUT): $(OBJS) $(INITTAR)
	$(LD) $(LDFLAGS) -e start -o $@ $(OBJS)
	$(OBJCOPY) -O binary $@
	dd if=/dev/zero \
	   status=none \
	   bs=1 \
	   count=$$(($(SYS_BINARY_SIZE) \
	             - $$(stat -c '%s' $@) % $(SYS_BINARY_SIZE))) >> $@
	cat $(INITTAR) >> $@

$(INIT):
	mkdir -p $@
	(cd .. && $(MAKE) ROOT=sys/$@ \
	                  SBIN_ENABLED_PROGRAMS='init' \
	                  root)

$(INITTAR): $(INIT)
	(cd $(INIT) && tar -c .) > $@

$(OBJS):
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
