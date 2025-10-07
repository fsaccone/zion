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

#define STACK_SIZE CEIL(8192, PAGE_SIZE)

#define VIRTUAL_INT_HANDLER   (0 * PAGE_SIZE)
#define VIRTUAL_TRAPFRAME     (1 * PAGE_SIZE)
#define VIRTUAL_PROGRAM_START (2 * PAGE_SIZE)
#define VIRTUAL_STACK_END     ((uptr)(~0))
#define VIRTUAL_STACK_START   (VIRTUAL_STACK_END - STACK_SIZE)

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
	struct pageoptions inthandleropts = {
		.u = 1,
		.r = 1,
		.w = 0,
		.x = 1,
	};
	struct pageoptions stackopts = {
		.u = 1,
		.r = 1,
		.w = 1,
		.x = 1,
	};
	struct pageoptions textopts = {
		.u = 1,
		.r = 1,
		.w = 0,
		.x = 1,
	};
	struct pageoptions tframeopts = {
		.u = 0,
		.r = 1,
		.w = 0,
		.x = 0,
	};
	uptr a;
	struct framenode *textfn;
	void *intbase;

	if (!(*p = palloc(sizeof(struct process), 0)))
		goto panic;

	if (unusedpid(&(*p)->pid)) {
		setpanicmsg("PID_MAX exceeded.");
		goto panic;
	}

	/* Allocate page tree. */
	if (!((*p)->pagetree = allocpagetable()))
		goto panic;

	/* Set other initial values. */
	(*p)->state = READY;
	(*p)->children = NULL;

	/* Allocate and map stack. */
	for (a = 0; a < STACK_SIZE; a += PAGE_SIZE) {
		void *f;
		struct framenode *fn;

		if (!(f = palloc(PAGE_SIZE, 0)))
			goto panic;

		/* Append f to *p->allocated. */
		if (!(fn = palloc(sizeof(struct framenode), 0)))
			goto panic;
		fn->f = f;
		fn->n = (*p)->allocated;
		(*p)->allocated = fn;

		if (vmap((*p)->pagetree,
		         VIRTUAL_STACK_START + a,
		         f,
		         stackopts)) {
			goto panic;
		}
	}

	/* Map text. */
	a = 0;
	for (textfn = text; textfn; textfn = textfn->n) {
		if (vmap((*p)->pagetree,
		         VIRTUAL_PROGRAM_START + a,
		         textfn->f,
		         textopts))
			goto panic;

		a += PAGE_SIZE;
	}

	/* Allocate and map trap frame. */
	if (!((*p)->trapframe = palloc(PAGE_SIZE, 0)))
		goto panic;
	if (vmap((*p)->pagetree,
	         VIRTUAL_TRAPFRAME,
	         (*p)->trapframe,
	         tframeopts))
		goto panic;

	/* Map interrupt handler. */
	intbase = userinterruptbase();
	if ((uptr)intbase % PAGE_SIZE) {
		setpanicmsg("User interrupt handler not aligned to"
		            " PAGE_SIZE.");
		goto panic;
	}
	if (vmap((*p)->pagetree,
	         VIRTUAL_INT_HANDLER,
	         intbase,
	         inthandleropts))
		goto panic;

	/* Set program counter and stack pointer. */
	setctxpc((*p)->uctx, (void *)VIRTUAL_PROGRAM_START);
	setctxsp((*p)->uctx, (void *)VIRTUAL_STACK_END);

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
