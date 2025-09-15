include ../../config.mk

OUT = init

OBJS = init.o

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)
