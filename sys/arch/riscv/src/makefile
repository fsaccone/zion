include ../../../../config.mk

OUT  = arch.a
OBJS = boot.o

ASCMD = $(AS) -o $@ $^

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

boot.o: boot.s
	$(ASCMD)
