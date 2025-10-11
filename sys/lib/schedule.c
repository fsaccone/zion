#include <schedule.h>

#include <arch/page.h>
#include <console.h>
#include <core.h>
#include <ctx.h>
#include <interrupt.h>
#include <machine/cpu.h>
#include <process.h>
#include <spinlock.h>
#include <trampoline.h>
#include <vmem.h>

static struct process *coreprocesses[NCPU] = { 0 };

struct process *
coreprocess(void)
{
	return coreprocesses[core()];
}

void
nextschedule(void)
{
	u16 c = core();

	if (!coreprocesses[c] || coreprocesses[c]->state != RUNNING)
		return;

	coreprocesses[c]->state = READY;

	switchctx(coreprocesses[c]->uctx, coreprocesses[c]->kctx);
}

void
schedule(void)
{
	u16 c = core();
	struct processnode *pn;

	for(;;) {
		u8 found = 0;

		for (pn = processes(); pn; pn = pn->n) {
			struct process *p = pn->p;

			lock(&p->lock);

			switch (p->state) {
			case READY:
				p->state = RUNNING;

				/* After setting the process state to RUNNING,
				   unlock all the other cores: they will skip
				   this process and find another one. */
				unlock(&p->lock);

				coreprocesses[c] = p;

				/* Since inittrapframe changes the context to
				   make the jump to usermode possible, it is
				   needed to disable interrupts to avoid
				   messing up. */
				disableinterrupts();

				inittrapframe(paddr(p->pagetree,
				                    VADDR_TRAP_FRAME),
				              p->pagetree, getctxpc(p->uctx));
				setctxpc(p->uctx, usermodebase());
				switchctx(p->kctx, p->uctx);

				found = 1;

				break;
			default:
			}

			unlock(&p->lock);
		}

		/* No process, out of all, was found for running core. */
		if (!found)
			waitforinterrupt();
	}
}
