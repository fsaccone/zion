include ../../../../config.mk

OUT  = boot.o
OBJS = start.o

ASCMD = $(AS) -o $@ $^

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

start.o: start.s
	$(ASCMD)

.PHONY: clean
