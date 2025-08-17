include ../../config.mk

OUT  = sys.a
OBJS = kmain.o

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

kmain.o: kmain.c
	$(CC) $(CFLAGS) -I../arch/$(ARCH)/include -c -o $@ $^

.PHONY: clean
