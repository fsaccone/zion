include ../config.mk
include objs.mk

OUT = sys.bin

.PHONY: all clean test

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS) $(OBJS:.o=.c.o) $(OBJS:.o=.s.o)

test: $(OUT)
	$(QEMU) $(QEMUFLAGS) -bios none \
	                     -kernel $(OUT)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -e start -o $@ $^
	$(STRIP) $@
	$(OBJCOPY) -O binary $@

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
