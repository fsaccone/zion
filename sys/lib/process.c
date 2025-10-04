#include <process.h>

#include <ctx.h>
#include <machine/cpu.h>
#include <panic.h>
#include <pmem.h>
#include <vmem.h>

/* Dequeues a process from queue at address q and sets pointer p to it. The p
   pointer is set to NULL if the queue is empty. Returns -1 in case of error or
   0 otherwise. */
static s8 dequeue(struct process **p, struct processlist **q);

/* Enqueues process p to queue at address q, Returns 0 normally and -1 in case
   of error. */
static s8 enqueue(struct process *p, struct processlist **q);

/* Returns the first unused PID from pidbitmap and sets it to used. Returns 0
   if pidbitmap is full. */
static u16 unusedpid(void);

static struct processlist *createdqueue           = NULL;
static struct process      init                   = { 0 };
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
allocprocess(void *pbase, struct process *parent)
{
	struct process *p;
	struct processlist *child;

	if (!(p = palloc(sizeof(struct process), 0))) {
		tracepanicmsg("createprocess");
		return -1;
	}

	if (!(child = palloc(sizeof(struct processlist), 0))) {
		tracepanicmsg("createprocess");
		return -1;
	}

	if (!(p->pid = unusedpid())) {
		setpanicmsg("PID_MAX exceeded.");
		tracepanicmsg("createprocess");
		return -1;
	}

	p->state = CREATED;
	p->children = NULL;
	setctxpc(p->ctx, pbase);

	child->p = p;

	child->n = parent->children;
	parent->children = child;

	if (enqueue(p, &createdqueue)) {
		tracepanicmsg("createprocess");
		return -1;
	}

	return 0;
}

s8
initprocess(void *pbase)
{
	/* Set bit 0 to used. */
	pidbitmap[0] |= 1;

	init.pid = 0;
	init.state = CREATED;
	init.children = NULL;
	setctxpc(init.ctx, pbase);

	if (enqueue(&init, &createdqueue)) {
		tracepanicmsg("initprocess");
		return -1;
	}

	return 0;
}
