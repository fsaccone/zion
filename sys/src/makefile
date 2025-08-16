include ../config.mk

OUT  = sys.a
OBJS = kmain.o

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

kmain.o: kmain.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
