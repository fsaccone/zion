#include <schedule.h>

#include <arch/ctx.h>
#include <arch/page.h>
#include <console.h>
#include <core.h>
#include <ctx.h>
#include <interrupt.h>
#include <machine/cpu.h>
#include <panic.h>
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

	if (!coreprocesses[c])
		return;

	if (!holding(&coreprocesses[c]->lock)) {
		setpanicmsg("Process lock not acquired.");
		tracepanicmsg("nextschedule");
		panic();
	}

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

				/* Since preparetrapframe changes the context
				   to make the jump to usermode possible, it is
				   needed to disable interrupts to avoid
				   messing up. */
				disableinterrupts();

				preparetrapframe(trapframe(p), p->pagetree);
				setctxpc(p->ctx,
				         usermodebase(PROC_VAS_TRAMPOLINE));
				switchctx(corectxs[c], p->ctx);

				/* A core may come back here by calling
				   nextschedule. Since the lock of the process
				   must be acquired to call nextschedule, we
				   need to release it. */
				unlock(&p->lock);

				/* Only change the state back to READY if it
				   was not already modified. */
				if (p->state == RUNNING)
					p->state = READY;

				break;
			case TERMINATED:
				/* Remove the process from the linked list. */
				removeprocess(p);

				/* Set state to BLOCKED and unlock the process
				   so that other cores may skip it. */
				p->state = BLOCKED;
				unlock(&p->lock);

				/* We are now sure that all the cores which
				   checked for this process have acquired the
				   lock, and that no other core has come into
				   contact with this process since it was
				   removed from the linked list.
				   Lock the process again to wait for the other
				   cores to skip the process. */
				lock(&p->lock);

				/* Unlock the process again. This is pretty
				   much not needed but it is more clear like
				   this. */
				unlock(&p->lock);

				/* Free the process. If the freeing fails, we
				   do not take any action. */
				(void)freeprocess(p);

				break;
			default:
				unlock(&p->lock);
			}
		}
	}
}
