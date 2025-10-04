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
	void *b = tarbase(init);
	uptr a, s = tarsize(init);

	/* Save content of init in the text linked list of frames. */
	for (a = 0; a < s; a += PAGE_SIZE) {
		struct framenode *fn;
		void *f;

		if (!(fn = palloc(sizeof(struct framenode), 0)))
			goto panic;

		if (!(f = palloc(PAGE_SIZE, 0)))
			goto panic;

		pmemcpy(f, (void *)((uptr)b + a), a > s
		                                  ? PAGE_SIZE - (a - s)
		                                  : PAGE_SIZE);

		fn->f = f;
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
