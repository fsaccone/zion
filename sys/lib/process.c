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
static struct process *init = NULL;
static u8 pidbitmap[CEIL(PID_MAX, 8) / 8] = { 0 };

struct process *
allocprocess(struct process *parent)
{
	struct process *p;
	struct pageoptions opts = { 0 };
	void *tframe;

	if (!parent && init) {
		setpanicmsg("Tried allocating init process twice.");
		goto panic;
	}

	if (!(p = palloc(sizeof(struct process), 0)))
		goto panic;

	/* Set parent or init. */
	if (parent)
		p->parent = parent;
	else
		init = p;

	/* Set initial state. */
	p->state = BLOCKED;

	/* Allocate smallest free PID. */
	for (p->pid = 1; p->pid < PID_MAX; p->pid++) {
		if (BITMAPGET(pidbitmap, p->pid - 1))
			continue;

		BITMAPADD(pidbitmap, p->pid - 1);

		break;
	}

	if (p->pid == PID_MAX) {
		setpanicmsg("Maximum number of processes reached.");
		goto panic;
	}

	/* Allocate page tree. */
	if (!(p->pagetree = allocpagetable()))
		goto panic;

	/* Allocate, initialize and map trap frame. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 1;
	opts.x = 0;
	opts.a = 1;
	if (!(tframe = palloc(PAGE_SIZE, 0)))
		goto panic;
	inittrapframe(tframe, PROC_VAS_FIRST_FREE_PAGE, PROC_VAS_TRAMPOLINE);
	if (vmap(p->pagetree, PROC_VAS_TRAP_FRAME, tframe, opts))
		goto panic;

	/* Map trampoline. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 0;
	opts.x = 1;
	opts.a = 0;
	if (vmap(p->pagetree, PROC_VAS_TRAMPOLINE, trampolinebase(), opts))
		goto panic;

	/* Set ceil to the first free page. */
	p->ceil = PROC_VAS_FIRST_FREE_PAGE;

	return p;

panic:
	tracepanicmsg("allocprocess");
	return NULL;
}

void
appendprocess(struct process *p)
{
	p->n = processlist;

	if (processlist)
		processlist->p = p;

	processlist = p;
}

s8
freeprocess(struct process *p)
{
	void *tframe;
	uptr a;

	/* Free and unmap trap frame. */
	if ((tframe = trapframe(p))) {
		if (pfree(tframe, PAGE_SIZE))
			goto panic;

		if (vunmap(p->pagetree, PROC_VAS_TRAP_FRAME))
			goto panic;
	}

	/* Free and unmap all user pages. */
	for (a = PROC_VAS_FIRST_FREE_PAGE; a < p->ceil; a += PAGE_SIZE) {
		struct pageoptions opts;
		void *f;

		if (!(f = paddr(&opts, p->pagetree, a)))
			continue;

		if (opts.a && pfree(f, PAGE_SIZE))
			goto panic;

		if (vunmap(p->pagetree, a))
			goto panic;
	}

	/* Now that the page tree contains no allocated page tables, it is
	   possible to free it. */
	if (freepagetable(p->pagetree))
		goto panic;

	/* Finally free the process. */
	if (pfree(p, sizeof(struct process)))
		goto panic;

	return 0;

panic:
	tracepanicmsg("freeprocess");
	return -1;
}

s8
growprocess(uptr *o, struct process *p, void *f, struct pageoptions opts)
{
	/* Map page. */
	if (vmap(p->pagetree, p->ceil, f, opts))
		goto panic;

	/* Set o. */
	if (o)
		*o = p->ceil;

	/* Increase ceil. */
	if (PAGE_VADDR_MAX - p->ceil < PAGE_SIZE) {
		setpanicmsg("Max virtual address reached by process ceil.");
		goto panic;
	}
	p->ceil += PAGE_SIZE;

	return 0;

panic:
	tracepanicmsg("growprocess");
	return -1;
}

struct process *
initprocess(void)
{
	return init;
}

struct process *
processes(void)
{
	return processlist;
}

void
removeprocess(struct process *p)
{
	if (p->n)
		p->n->p = p->p;
}

void *
trapframe(struct process *p)
{
	if (!p)
		return NULL;

	return paddr(NULL, p->pagetree, PROC_VAS_TRAP_FRAME);
}
