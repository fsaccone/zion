#include <syscall.h>

#include <panic.h>
#include <process.h>
#include <schedule.h>

void
exit(void)
{
	struct process *p, *init, *c;

	if (!(p = coreprocess()))
		return;

	if (!(init = initprocess()))
		return;

	if (p == init) {
		setpanicmsg("Init process attempted to exit.");
		tracepanicmsg("exit");
		panic();
	}

	/* Reparent orphaned processes to init. */
	for (c = processes(); c; c = c->n) {
		if (c->parent == p)
			c->parent = init;
	}

	p->state = TERMINATED;

	/* Jump to scheduler. */
	nextschedule();
}
