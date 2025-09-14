#include <console.h>
#include <interrupt.h>
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
		consolewrite("interrupt: Hardware.\n");
		releaselock(&l);
		break;
	case INTERRUPT_TYPE_SOFTWARE:
		acquirelock(&l);
		consolewrite("interrupt: Software.\n");
		releaselock(&l);
		break;
	case INTERRUPT_TYPE_TIMER:
		setupnexttimer();
		break;
	default:
		acquirelock(&l);
		consolewrite("interrupt: Unknown.\n");
		releaselock(&l);
	}
}
