#include <process.h>

#include <ctx.h>
#include <machine/cpu.h>
#include <panic.h>
#include <pmem.h>
#include <vmem.h>

#define VIRTUAL_PROGRAM_START 0x0
#define VIRTUAL_STACK_END     ((uptr)(~0))
#define VIRTUAL_STACK_START   (VIRTUAL_STACK_END - STACK_SIZE)

/* Dequeues a process from queue at address q and sets pointer p to it. The p
   pointer is set to NULL if the queue is empty. Returns -1 in case of error or
   0 otherwise. */
static s8 dequeue(struct process **p, struct processlist **q);

/* Enqueues process p to queue at address q, Returns 0 normally and -1 in case
   of error. */
static s8 enqueue(struct process *p, struct processlist **q);

/* Returns the first unused PID from pidbitmap and sets it to used. Returns 0
   on the first call or if pidbitmap is full. */
static u16 unusedpid(void);

static struct processlist *createdqueue           = NULL;
static u8                  initdone               = 0;
static struct process     *coreprocesses[NCPU]    = { 0 };
static u8                  pidbitmap[PID_MAX / 8] = { 0 };

s8
dequeue(struct process **p, struct processlist **q)
{
	struct processlist *tail;

	if (!*q) {
		*p = NULL;
		return 0;
	}

	/* Find tail of q. */
	for (tail = *q; tail->n; tail = tail->n);

	*p = tail->p;

	if (pfree(tail, sizeof(struct processlist)))
		return -1;

	return 0;
}

s8
enqueue(struct process *p, struct processlist **q)
{
	struct processlist *new, *tail;

	if (!(new = palloc(sizeof(struct processlist), 0)))
		return -1;

	new->p = p;
	new->n = NULL;

	if (!*q) {
		*q = new;
		return 0;
	}

	/* Find tail of q. */
	for (tail = *q; tail->n; tail = tail->n);

	tail->n = new;

	return 0;
}

u16
unusedpid(void)
{
	u16 i;

	for (i = 0; i < PID_MAX; i++) {
		if (!(pidbitmap[i / 8] & (1 << (i % 8)))) {
			pidbitmap[i / 8] |= (1 << (i % 8));
			return i;
		}
	}

	return 0;
}

s8
allocprocess(void *pbase, uptr psize, struct process *parent)
{
	struct process *p;
	struct pageoptions textopts = {
		.u = 1,
		.r = 1,
		.w = 0,
		.x = 1,
	};
	uptr a;

	if (!(p = palloc(sizeof(struct process), 0))) {
		tracepanicmsg("allocprocess");
		return -1;
	}

	/* Set pid, check if it returns 0 after the init process was already
	   created. */
	if (!(p->pid = unusedpid()) && initdone) {
		setpanicmsg("PID_MAX exceeded.");
		tracepanicmsg("allocprocess");
		return -1;
	}

	/* Allocate page tree. */
	if (!(p->pagetree = allocpagetable())) {
		tracepanicmsg("allocprocess");
		return -1;
	}

	/* Set other initial values. */
	p->state = CREATED;
	p->children = NULL;

	/* Require pbase to be aligned to PAGE_SIZE. */
	if ((uptr)pbase % PAGE_SIZE) {
		setpanicmsg("Passed pbase not aligned to PAGE_SIZE.");
		tracepanicmsg("allocprocess");
		return -1;
	}

	/* Map text. */
	for (a = 0; a < psize; a += PAGE_SIZE) {
		if (vmap(p->pagetree,
		         VIRTUAL_PROGRAM_START + a,
		         (void *)((uptr)pbase + a),
		         textopts)) {
			tracepanicmsg("allocprocess");
			return -1;
		}
	}

	/* Set program counter. */
	setctxpc(p->ctx, VIRTUAL_PROGRAM_START);

	/* Append to parent if it is not the init process. */
	if (parent) {
		if (enqueue(p, &parent->children)) {
			tracepanicmsg("allocprocess");
			return -1;
		}
	} else if (initdone) {
		setpanicmsg("Init process allocated twice.");
		tracepanicmsg("allocprocess");
		return -1;
	} else {
		initdone = 1;
	}

	/* Append to the queue. */
	if (enqueue(p, &createdqueue)) {
		tracepanicmsg("allocprocess");
		return -1;
	}

	return 0;
}
