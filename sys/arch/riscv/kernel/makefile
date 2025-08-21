include ../../../../config.mk

OUT  = kernel.o
OBJS = ctx.o start.o

ASCMD = $(AS) -o $@ $^

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

ctx.o: ctx.s
	$(ASCMD)

start.o: start.s
	$(ASCMD)
