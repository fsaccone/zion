include config.mk

SYS = sys.bin

.PHONY: all clean help sys

all: help

clean:
	rm -f $(SYS)

help:
	@printf "Usage:\n"
	@printf "\t$(MAKE) clean - Clean the whole codebase.\n"
	@printf "\t$(MAKE) sys   - Build the kernel binary as $(SYS).\n"

sys: $(SYS)

$(SYS):
	(cd sys && $(MAKE) $@)
	cp sys/$@ .
