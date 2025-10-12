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

/* Allocates a frame, sets f to its address and appends it to the allocated
   linked list of process p. Returns -1 in case of error or 0 otherwise. */
static s8 allocprocpage(void **f, struct process *p);

/* Allocates process and sets pointer p to it after initializing it. Returns -1
   in case of error or 0 otherwise */
static s8 allocprocess(struct process **p);

static struct processnode *processlist            = NULL;
static u8                  pidbitmap[CEIL(PID_MAX, 8) / 8] = { 0 };

s8
allocprocpage(void **f, struct process *p)
{
	struct framenode *fn;

	if (!(*f = palloc(PAGE_SIZE, 0)))
		goto panic;

	if (!(fn = palloc(sizeof(struct framenode), 0)))
		goto panic;

	fn->f = *f;

	fn->n = p->allocated;
	p->allocated = fn;

	return 0;

panic:
	tracepanicmsg("allocprocpage");
	return -1;
}

s8
allocprocess(struct process **p)
{
	struct pageoptions popts = { 0 };
	void *tframe;

	if (!(*p = palloc(sizeof(struct process), 0)))
		goto panic;

	/* Allocate smallest free PID. */
	for ((*p)->pid = 0; (*p)->pid < PID_MAX; (*p)->pid++) {
		if (BITMAPGET(pidbitmap, (*p)->pid))
			continue;

		BITMAPADD(pidbitmap, (*p)->pid);

		break;
	}

	if ((*p)->pid == PID_MAX) {
		setpanicmsg("PID_MAX exceeded.");
		goto panic;
	}

	/* Set other initial values. */
	(*p)->state = READY;
	(*p)->children = NULL;

	/* Allocate page tree. */
	if (!((*p)->pagetree = allocpagetable()))
		goto panic;

	/* Map trampoline. */
	popts.u = 0;
	popts.r = 1;
	popts.w = 0;
	popts.x = 1;
	if (vmap((*p)->pagetree, PROC_VAS_TRAMPOLINE, trampolinebase(), popts))
		goto panic;

	/* Allocate, initialize and map trap frame. */
	popts.u = 0;
	popts.r = 1;
	popts.w = 1;
	popts.x = 0;
	if (allocprocpage(&tframe, *p))
		goto panic;
	inittrapframe(tframe, PROC_VAS_FIRST_FREE_PAGE, PROC_VAS_TRAMPOLINE);
	if (vmap((*p)->pagetree, PROC_VAS_TRAP_FRAME, tframe, popts))
		goto panic;

	return 0;

panic:
	tracepanicmsg("allocprocess");
	return -1;
}

s8
createprocess(struct process **p, struct process *parent)
{
	struct processnode *pn;

	if (allocprocess(p))
		goto panic;

	if (parent) {
		struct processnode *newchild;

		if (!(newchild = palloc(sizeof(struct processnode), 0)))
			goto panic;

		newchild->p = *p;

		newchild->n = parent->children;
		parent->children = newchild;
	}

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
