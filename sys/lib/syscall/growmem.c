#include <syscall.h>

#include <arch/page.h>
#include <math.h>
#include <pmem.h>
#include <process.h>
#include <schedule.h>
#include <trampoline.h>

void
growmem(uptr s)
{
	struct process *p;
	struct pageoptions opts = {
		.u = 1,
		.r = 1,
		.w = 1,
		.x = 0,
		.a = 1,
	};
	uptr npages = CEIL(s, PAGE_SIZE) / PAGE_SIZE;
	void *first = NULL;

	if (!s)
		goto fail;

	if (!(p = coreprocess()))
		goto fail;

	while (npages--) {
		void *f;
		uptr a;

		if (!(f = palloc(PAGE_SIZE, 0)))
			goto fail;

		if (growprocess(&a, p, f, opts))
			goto fail;

		if (!first)
			first = (void *)a;
	}

	setreturn(trapframe(p), (ureg)first);

	return;

fail:
	if (p)
		setreturn(trapframe(p), (ureg)NULL);
}
