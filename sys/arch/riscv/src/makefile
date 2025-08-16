include ../../../config.mk

OUT  = arch.a
OBJS = boot.o

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(AR) -r $@ $^

boot.o: boot.s
	$(AS) -o $@ $^

.PHONY: clean
