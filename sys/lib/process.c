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

/* Allocates a frame, sets f to its address and appends it to the vas linked
   list of process p using v as the virtual address mapped to f. Returns -1 in
   case of error or 0 otherwise. */
static s8 allocprocpage(void **f, uptr v, struct process *p);

static struct processnode *processlist            = NULL;
static u8                  pidbitmap[CEIL(PID_MAX, 8) / 8] = { 0 };

s8
allocprocpage(void **f, uptr v, struct process *p)
{
	struct pagenode *pn;

	if (!(*f = palloc(PAGE_SIZE, 0)))
		goto panic;

	if (!(pn = palloc(sizeof(struct pagenode), 0)))
		goto panic;

	pn->f = *f;
	pn->p = v;

	pn->n = p->vas;
	p->vas = pn;

	return 0;

panic:
	tracepanicmsg("allocprocpage");
	return -1;
}

s8
createprocess(struct process **p, struct process *parent)
{
	struct pageoptions opts = { 0 };
	void *tframe;
	struct processnode *pn;

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
	if (allocprocpage(&tframe, PROC_VAS_FIRST_FREE_PAGE, *p))
		goto panic;
	inittrapframe(tframe, PROC_VAS_FIRST_FREE_PAGE, PROC_VAS_TRAMPOLINE);
	if (vmap((*p)->pagetree, PROC_VAS_TRAP_FRAME, tframe, opts))
		goto panic;

	(*p)->parent = parent;

	if (!(pn = palloc(sizeof(struct processnode), 0)))
		goto panic;

	pn->p = *p;

	pn->n = processlist;
	processlist = pn;

	return 0;

panic:
	tracepanicmsg("createprocess");
	return -1;
}

struct processnode *
processes(void)
{
	return processlist;
}

void *
trapframe(struct process *p)
{
	return paddr(p->pagetree, PROC_VAS_TRAP_FRAME);
}
