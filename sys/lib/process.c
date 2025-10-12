#include <process.h>

#include <arch/page.h>
#include <arch/types.h>
#include <ctx.h>
#include <machine/cpu.h>
#include <math.h>
#include <panic.h>
#include <pmem.h>
#include <trampoline.h>
#include <vmem.h>

static struct process *processlist = NULL;
static u8 pidbitmap[CEIL(PID_MAX, 8) / 8] = { 0 };

s8
allocprocess(struct process **p, struct process *parent)
{
	struct pageoptions opts = { 0 };
	void *tframe;

	if (!(*p = palloc(sizeof(struct process), 0)))
		goto panic;

	(*p)->state = READY;

	/* Allocate smallest free PID. */
	for ((*p)->pid = 0; (*p)->pid < PID_MAX; (*p)->pid++) {
		if (BITMAPGET(pidbitmap, (*p)->pid))
			continue;

		BITMAPADD(pidbitmap, (*p)->pid);

		break;
	}

	if ((*p)->pid == PID_MAX) {
		setpanicmsg("Maximum number of processes reached.");
		goto panic;
	}

	/* Allocate page tree. */
	if (!((*p)->pagetree = allocpagetable()))
		goto panic;

	/* Map trampoline. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 0;
	opts.x = 1;
	if (vmap((*p)->pagetree, PROC_VAS_TRAMPOLINE, trampolinebase(), opts))
		goto panic;

	/* Allocate, initialize and map trap frame. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 1;
	opts.x = 0;
	if (!(tframe = palloc(PAGE_SIZE, 0)))
		goto panic;
	inittrapframe(tframe, PROC_VAS_FIRST_FREE_PAGE, PROC_VAS_TRAMPOLINE);
	if (vmap((*p)->pagetree, PROC_VAS_TRAP_FRAME, tframe, opts))
		goto panic;

	(*p)->parent = parent;

	/* Append to processlist. */
	(*p)->n = processlist;
	processlist = *p;

	return 0;

panic:
	tracepanicmsg("allocprocess");
	return -1;
}

struct process *
processes(void)
{
	return processlist;
}

void *
trapframe(struct process *p)
{
	return paddr(p->pagetree, PROC_VAS_TRAP_FRAME);
}
