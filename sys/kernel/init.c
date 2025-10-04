#include "init.h"

#include <arch/types.h>
#include <math.h>
#include <panic.h>
#include <pmem.h>
#include <process.h>
#include <tar.h>

s8
createinitprocess(struct tarheader *init)
{
	struct framenode *text = NULL, *texttail = NULL;
	uptr a;

	/* Save content of init in the text linked list of frames. */
	for (a = 0; a < tarsize(init); a += PAGE_SIZE) {
		struct framenode *fn;

		if (!(fn = palloc(sizeof(struct framenode), 0)))
			goto panic;

		fn->f = (void *)(MIN((uptr)tarbase(init) + a,
		                     (uptr)tarbase(init) + tarsize(init)));
		fn->n = NULL;

		/* Append fn to text. */
		if (!texttail)
			text = fn;
		else
			texttail->n = fn;
		texttail = fn;
	}

	if (createprocess(text, NULL))
		goto panic;

	return 0;

panic:
	tracepanicmsg("coremain");
	return -1;
}
