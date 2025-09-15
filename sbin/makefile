PROGRAMS = init

.PHONY: all clean $(PROGRAMS)

all: $(PROGRAMS)

clean:
	for p in $(PROGRAMS); do (cd init && $(MAKE) clean); done

$(PROGRAMS):
	(cd $@ && $(MAKE) $@)
