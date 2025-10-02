#include <process.h>

#include <machine/cpu.h>
#include <panic.h>
#include <pmem.h>
#include <vmem.h>

/* Enqueues process p to queue at address q, Returns 0 normally and -1 in case
   of error. */
static u8 enqueue(struct process *p, struct processlist **q);

/* Returns the first unused PID from pidbitmap and sets it to used. Returns 0
   if pidbitmap is full. */
static u16 unusedpid(void);

static struct processlist *createdqueue           = NULL;
static struct process      init                   = { 0 };
static struct process     *coreprocesses[NCPU]    = { 0 };
static u8                  pidbitmap[PID_MAX / 8] = { 0 };

u8
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

struct process *
createprocess(struct process *parent)
{
	struct process *p;
	struct processlist *child;

	if (!(p = palloc(sizeof(struct process), 0))) {
		tracepanicmsg("createprocess");
		return NULL;
	}

	if (!(child = palloc(sizeof(struct processlist), 0))) {
		tracepanicmsg("createprocess");
		return NULL;
	}

	if (!(p->pid = unusedpid())) {
		setpanicmsg("PID_MAX exceeded.");
		tracepanicmsg("createprocess");
		return NULL;
	}

	p->state = CREATED;
	p->children = NULL;

	child->p = p;

	child->n = parent->children;
	parent->children = child;

	if (enqueue(p, &createdqueue)) {
		tracepanicmsg("createprocess");
		return NULL;
	}

	return p;
}

struct process *
initprocess(void)
{
	/* Set bit 0 to used. */
	pidbitmap[0] |= 1;

	init.pid = 0;
	init.state = CREATED;
	init.children = NULL;

	if (enqueue(&init, &createdqueue)) {
		tracepanicmsg("initprocess");
		return NULL;
	}

	return &init;
}
