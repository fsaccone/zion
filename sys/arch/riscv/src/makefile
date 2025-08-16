include ../../../config.mk

OUT  = arch.a
OBJS = boot.o

$(OUT): $(OBJS)
	$(AR) -r $@ $^

boot.o: boot.s
	$(AS) -o $@ $^
