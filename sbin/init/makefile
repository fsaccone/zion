include ../../config.mk

OUT = init

OBJS = init.o

.PHONY: all clean install

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

install: $(OUT)
	mkdir -p $(PREFIX)/sbin
	cp -f $(OUT) $(PREFIX)/sbin
	chmod 755 $(PREFIX)/sbin

$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)
