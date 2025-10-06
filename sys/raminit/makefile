include ../../config.mk

OBJS = raminit.o

OUT = raminit.bin

.PHONY: all clean

all: $(OUT)

clean:
	rm -rf $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -e raminit -o $@ $^
	$(OBJCOPY) -O binary $@

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)
