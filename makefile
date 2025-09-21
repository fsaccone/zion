include config.mk
include targets.mk

SYS = sys.bin

.PHONY: all clean help sys $(TARGETS) $(TARGETS:=/install)

all: help

clean:
	rm -f $(SYS)
	for t in $(TARGETS); do (cd $$t && $(MAKE) clean); done

help:
	@printf "Usage:\n"
	@printf "\t$(MAKE) clean              - Clean the whole codebase.\n"
	@printf "\t$(MAKE) sys                - Build the kernel binary as $(SYS).\n"
	@printf "\t$(MAKE) DIR/TARGET         - Build TARGET target of /DIR/.\n"
	@printf "\t$(MAKE) DIR/TARGET/install - Install built DIR/TARGET to PREFIX.\n"

sys: $(SYS)

$(SYS):
	(cd sys && $(MAKE) $@)
	cp sys/$@ .

$(TARGETS):
	(cd $@ && $(MAKE) all > /dev/null 2>&1)

$(TARGETS:=/install):
	(cd $(@:/install=) && $(MAKE) PREFIX=.tmp install)
	mkdir -p $(PREFIX)
	mv $(@:/install=)/.tmp/* $(PREFIX)
	rm -rf $(@:/install=)/.tmp
