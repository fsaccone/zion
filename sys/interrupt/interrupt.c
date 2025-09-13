#include <interrupt.h>
#include <log.h>
#include <spinlock.h>
#include <timer.h>

#include "syscall.h"

static struct lock l = { 0 };

void
interrupt(void)
{
	un *args = interruptargs();

	switch (interrupttype()) {
	case INTERRUPT_TYPE_SYSCALL:
		acquirelock(&l);
		if (syscall((u16)args[0], &args[1]))
			panic("interrupt");
		releaselock(&l);
		return;
	case INTERRUPT_TYPE_EXCEPTION:
		acquirelock(&l);
		setpanicmsg("Exception.");
		panic("interrupt");
		releaselock(&l);
		break;
	case INTERRUPT_TYPE_HARDWARE:
		acquirelock(&l);
		debug("interrupt: Hardware.\n");
		releaselock(&l);
		break;
	case INTERRUPT_TYPE_SOFTWARE:
		acquirelock(&l);
		debug("interrupt: Software.\n");
		releaselock(&l);
		break;
	case INTERRUPT_TYPE_TIMER:
		setupnexttimer();
		acquirelock(&l);
		debug("interrupt: Timer.\n");
		releaselock(&l);
		break;
	default:
		acquirelock(&l);
		debug("interrupt: Unknown.\n");
		releaselock(&l);
	}
}
