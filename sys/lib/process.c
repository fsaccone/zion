#include <process.h>

#include <arch/page.h>
#include <arch/types.h>
#include <ctx.h>
#include <machine/cpu.h>
#include <math.h>
#include <panic.h>
#include <pmem.h>
#include <user.h>
#include <vmem.h>

/* Allocates process and sets pointer p to it after initializing it using text
   as the frames containing the text of the program. Returns -1 in case of
   error or 0 otherwise */
static s8 allocprocess(struct process **p, struct framenode *text);

/* Finds the first unused PID from pidbitmap, sets it to used and sets o to it.
   Returns 1 if the bit map is full or 0 otherwise. */
static u8 unusedpid(u16 *o);

static struct processnode *processlist            = NULL;
static u8                  pidbitmap[PID_MAX / 8] = { 0 };

s8
allocprocess(struct process **p, struct framenode *text)
{
	struct pageoptions popts = { 0 };
	uptr a;
	struct framenode *textfn;

	if (!(*p = palloc(sizeof(struct process), 0)))
		goto panic;

	if (unusedpid(&(*p)->pid)) {
		setpanicmsg("PID_MAX exceeded.");
		goto panic;
	}

	/* Set other initial values. */
	(*p)->state = READY;
	(*p)->children = NULL;

	/* Allocate page tree. */
	if (!((*p)->pagetree = allocpagetable()))
		goto panic;

	/* Allocate trap frame. */
	if (!((*p)->trapframe = palloc(PAGE_SIZE, 0)))
		goto panic;

	/* Initialize virtual address space. */
	if (allocvas((*p)->pagetree, userinterruptbase(), (*p)->trapframe))
		goto panic;

	/* Map text. */
	popts.u = 1;
	popts.r = 1;
	popts.w = 0;
	popts.x = 1;
	a = 0;
	for (textfn = text; textfn; textfn = textfn->n) {
		if (vmap((*p)->pagetree, VADDR_FIRST_FREE_PAGE + a, textfn->f,
		         popts))
			goto panic;

		a += PAGE_SIZE;
	}

	/* Set program counter and stack pointer. */
	setctxpc((*p)->uctx, (void *)VADDR_FIRST_FREE_PAGE);
	setctxsp((*p)->uctx, (void *)VADDR_STACK_END);

	return 0;

panic:
	tracepanicmsg("allocprocess");
	return -1;
}

u8
unusedpid(u16 *o)
{
	u16 i;

	for (i = 0; i < PID_MAX; i++) {
		if (!(pidbitmap[i / 8] & (1 << (i % 8)))) {
			pidbitmap[i / 8] |= (1 << (i % 8));
			*o = i;
			return 0;
		}
	}

	return 1;
}

s8
createprocess(struct framenode *text, struct process *parent)
{
	struct process *p;
	struct processnode *pn;

	if (allocprocess(&p, text))
		goto panic;

	if (parent) {
		struct processnode *newchild;

		if (!(newchild = palloc(sizeof(struct processnode), 0)))
			goto panic;

		newchild->p = p;

		newchild->n = parent->children;
		parent->children = newchild;
	}

	if (!(pn = palloc(sizeof(struct processnode), 0)))
		goto panic;

	pn->p = p;

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
