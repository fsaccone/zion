#include <syscall.h>

#include <arch/page.h>
#include <math.h>
#include <pmem.h>
#include <process.h>
#include <schedule.h>

void *
growmem(uptr s)
{
	struct process *p;
	struct pageoptions opts = {
		.u = 1,
		.r = 1,
		.w = 1,
		.x = 0,
	};
	uptr npages = CEIL(s, PAGE_SIZE) / PAGE_SIZE;
	void *first = NULL;

	if (!s)
		goto error;

	if (!(p = coreprocess()))
		goto error;

	while (npages--) {
		void *f;
		uptr a;

		if (!(f = palloc(PAGE_SIZE, 0)))
			goto error;

		if (growprocess(&a, p, f, opts))
			goto error;

		if (!first)
			first = (void *)a;
	}

	return first;

error:
	return NULL;
}
