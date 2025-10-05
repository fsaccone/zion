#include <schedule.h>

#include <console.h>
#include <core.h>
#include <ctx.h>
#include <machine/cpu.h>
#include <process.h>
#include <spinlock.h>
#include <user.h>

u8              corectxs[NCPU][CTX_SIZE] = { 0 };
struct process *coreprocesses[NCPU]      = { 0 };

void
nextschedule(void)
{
	u16 c = core();

	if (!coreprocesses[c])
		return;

	switchctx(coreprocesses[c]->ctx, corectxs[c]);
}

void
schedule(void)
{
	u16 c = core();
	struct processnode *pn;

	for(;;) {
		for (pn = processes(); pn; pn = pn->n) {
			struct process *p = pn->p;

			lock(&p->lock);

			switch (p->state) {
			case READY:
				p->state = RUNNING;

				unlock(&p->lock);

				coreprocesses[c] = p;

				setuserptree(p->pagetree);
				setuserpc((uptr)getctxpc(p->ctx));
				setctxpc(p->ctx, usermode);
				switchctx(corectxs[c], p->ctx);

				break;
			default:
			}

			unlock(&p->lock);
		}
	}
}
