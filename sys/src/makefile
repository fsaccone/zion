include ../config.mk

OUT  = sys.a
OBJS = kmain.o

$(OUT): $(OBJS)
	$(AR) -r $@ $^

kmain.o: kmain.c
	$(CC) $(CFLAGS) -c -o $@ $^
