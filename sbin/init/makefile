include ../../config.mk

OUT = init

OBJS = init.o

.PHONY: all clean install uninstall

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

install: $(OUT)
	mkdir -p ../../out/sbin
	cp -f $(OUT) ../../out/sbin
	chmod 755 ../../out/sbin

uninstall:
	rm -f ../../out/sbin/$(OUT)

$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)
