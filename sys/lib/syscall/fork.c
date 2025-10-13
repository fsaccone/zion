#include <syscall.h>

#include <arch/types.h>
#include <pmem.h>
#include <process.h>
#include <schedule.h>
#include <trampoline.h>

void
fork(void)
{
	struct process *p, *c;
	void *ptframe, *ctframe;
	uptr a;

	if (!(p = coreprocess()))
		goto fail;

	/* Allocate child process. */
	if (!(c = allocprocess(p)))
		goto fail;

	/* Copy trap frame. */
	if (!(ptframe = trapframe(p)))
		goto fail;
	if (!(ctframe = trapframe(c)))
		goto fail;
	copytrapframe(ctframe, ptframe);

	/* Duplicate and map all user pages. */
	for (a = PROC_VAS_FIRST_FREE_PAGE; a < p->ceil; a += PAGE_SIZE) {
		void *df, *sf;
		struct pageoptions opts;
		uptr newa;

		if (!(sf = paddr(&opts, p->pagetree, a)))
			goto fail;

		if (!(df = palloc(PAGE_SIZE, 0)))
			goto fail;

		pmemcpy(df, sf, PAGE_SIZE);

		if (growprocess(&newa, c, df, opts))
			goto fail;

		if (newa != a)
			goto fail;
	}

	setreturn(ptframe, c->pid);
	setreturn(ctframe, 0);

	return;

fail:
	if (p && (ptframe = trapframe(p)))
		setreturn(ptframe, -1);

	if (c)
		(void)freeprocess(c);
}
