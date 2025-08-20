include ../../../../config.mk

OUT  = kernel.o
OBJS = start.o

ASCMD = $(AS) -o $@ $^

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT) $(OBJS)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

start.o: start.s
	$(ASCMD)
