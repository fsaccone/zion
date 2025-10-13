#include <schedule.h>

#include <arch/ctx.h>
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

static u8 corectxs[NCPU][CTX_SIZE] = { 0 };
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

	switchctx(coreprocesses[c]->ctx, corectxs[c]);
}

void
schedule(void)
{
	u16 c = core();

	for(;;) {
		struct process *p;
		for (p = processes(); p; p = p->n) {
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

				preparetrapframe(trapframe(p), p->pagetree);
				setctxpc(p->ctx,
				         usermodebase(PROC_VAS_TRAMPOLINE));
				switchctx(corectxs[c], p->ctx);

				/* Got back here through a nextschedule call
				   from the core which was owning the
				   process. */
				coreprocesses[c]->state = READY;

				break;
			default:
				unlock(&p->lock);
			}
		}
	}
}
