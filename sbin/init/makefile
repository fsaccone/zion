include ../../config.mk

OUT = init

OBJS = init.o

.PHONY: all clean install

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

install: $(OUT)
	mkdir -p ../../out/sbin
	cp -f $(OUT) ../../out/sbin
	chmod 755 ../../out/sbin

$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)
