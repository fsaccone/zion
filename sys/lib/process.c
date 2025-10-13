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
static struct process *initprocess = NULL;
static u8 pidbitmap[CEIL(PID_MAX, 8) / 8] = { 0 };

struct process *
allocprocess(struct process *parent)
{
	struct process *p;
	struct pageoptions opts = { 0 };
	void *tframe;

	if (!parent && initprocess) {
		setpanicmsg("Tried allocating init process twice.");
		goto panic;
	}

	if (!(p = palloc(sizeof(struct process), 0)))
		goto panic;

	if (parent)
		p->parent = parent;
	else
		initprocess = p;

	p->state = READY;

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

	/* Map trampoline. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 0;
	opts.x = 1;
	if (vmap(p->pagetree, PROC_VAS_TRAMPOLINE, trampolinebase(), opts))
		goto panic;

	/* Allocate, initialize and map trap frame. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 1;
	opts.x = 0;
	if (!(tframe = palloc(PAGE_SIZE, 0)))
		goto panic;
	inittrapframe(tframe, PROC_VAS_FIRST_FREE_PAGE, PROC_VAS_TRAMPOLINE);
	if (vmap(p->pagetree, PROC_VAS_TRAP_FRAME, tframe, opts))
		goto panic;

	/* Set ceil to the first free page. */
	p->ceil = PROC_VAS_FIRST_FREE_PAGE;

	/* Append to processlist. */
	p->n = processlist;
	if (processlist)
		processlist->p = p;
	processlist = p;

	return p;

panic:
	tracepanicmsg("allocprocess");
	return NULL;
}

s8
freeprocess(struct process *p)
{
	/* TODO: Unmap all pages in virtual address space, which also frees all
	         page tables. Frames pointed by pages also need to be freed. */

	/* Now that the page tree contains no allocated page tables, it is
	   possible to free it. */
	if (freepagetable(p->pagetree))
		goto panic;

	/* Make the previous process in the list point to the one after
	   this. */
	if (p->p)
		p->p->n = p->n;

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
processes(void)
{
	return processlist;
}

void *
trapframe(struct process *p)
{
	return paddr(NULL, p->pagetree, PROC_VAS_TRAP_FRAME);
}
