#include "raminit.h"

#include <config.h>
#include <machine/mem.h>
#include <panic.h>
#include <pmem.h>

s8
raminitframes(struct framenode **frames)
{
	uptr a;
	struct framenode *tail;

	if (RAMINIT_BASE % PAGE_SIZE) {
		setpanicmsg("Raminit program not aligned to PAGE_SIZE.");
		goto panic;
	}

	if (RAMINIT_BINARY_SIZE % PAGE_SIZE) {
		setpanicmsg("RAMINIT_BINARY_SIZE not aligned to PAGE_SIZE.");
		goto panic;
	}

	*frames = NULL;

	for (a = 0; a < RAMINIT_BINARY_SIZE; a += PAGE_SIZE) {
		struct framenode *fn;

		if (!(fn = palloc(sizeof(struct framenode), 0)))
			goto panic;

		fn->f = (void *)(RAMINIT_BASE + a);
		fn->n = NULL;

		if (!*frames)
			*frames = fn;
		else
			tail->n = fn;

		tail = fn;
	}

	return 0;

panic:
	tracepanicmsg("raminitframes");
	return -1;
}
