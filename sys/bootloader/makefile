include ../../config.mk

OUT  = bootloader.o
OBJS = bmain.o

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

$(OBJS):
	$(CC) $(CFLAGS) -I../arch/$(ARCH)/include -I../include -c \
	                -o $@ $(@:.o=.c)
