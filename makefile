include config.mk

.PHONY: all clean help

all: help

clean:

help:
	@printf "Usage:\n"
	@printf "\t$(MAKE) clean - Clean the whole codebase.\n"
