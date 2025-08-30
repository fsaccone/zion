include ../../config.mk

OUT  = sys.o
OBJS = kmain.o

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

$(OBJS):
	$(CC) $(CFLAGS) -I../machine/$(MACHINE)/include \
	                -I../arch/$(ARCH)/include \
	                -I../include -c \
	                -o $@ $(@:.o=.c)
