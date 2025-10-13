include ../../config.mk

CRT0 = crt0.o

.PHONY: all clean

all: $(CRT0)

clean:
	rm -f $(CRT0)
$(CRT0):
	$(AS) $(ASFLAGS) -o $@ arch/$(ARCH)/$(@:.o=.s)
