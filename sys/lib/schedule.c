#include <schedule.h>

#include <core.h>
#include <ctx.h>
#include <machine/cpu.h>
#include <process.h>

u8              corectxs[NCPU][CTX_SIZE] = { 0 };
struct process *coreprocesses[NCPU]      = { 0 };

void
nextschedule(void)
{
	u16 c = core();

	switchctx(coreprocesses[c]->ctx, corectxs[c]);

	return;
}

void
schedule(void)
{
	u16 c = core();
	struct processnode *pn;

	for(;;) {
		for (pn = processes(); pn; pn = pn->n) {
			struct process *p = pn->p;

			switch (p->state) {
			case READY:
				p->state = RUNNING;
				coreprocesses[c] = p;

				switchctx(corectxs[c], p->ctx);
				break;
			default:
			}
		}
	}
}
