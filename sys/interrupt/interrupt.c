#include <console.h>
#include <driver.h>
#include <interrupt.h>
#include <panic.h>
#include <spinlock.h>
#include <timer.h>

#include "syscall.h"

static struct lock l = { 0 };

void
interrupt(void)
{
	un *args = interruptargs();
	struct driver drv;

	switch (interrupttype()) {
	case INTERRUPT_TYPE_SYSCALL:
		if (syscall((u16)args[0], &args[1])) {
			tracepanicmsg("interrupt");
			panic();
		}

		return;
	case INTERRUPT_TYPE_EXCEPTION:
		acquirelock(&l);

		setpanicmsg("Exception.");
		tracepanicmsg("interrupt");
		panic();

		releaselock(&l);
		break;
	case INTERRUPT_TYPE_HARDWARE:
		if (interruptdriver(&drv)) {
			setpanicmsg("Unknown hardware interrupt.");
			tracepanicmsg("interrupt");
			panic();
		}

		if (drv.interrupt)
			drv.interrupt();

		break;
	case INTERRUPT_TYPE_SOFTWARE:
		(void)consolewrite("interrupt: Software.\n");
		break;
	case INTERRUPT_TYPE_TIMER:
		setupnexttimer();
		break;
	default:
		(void)consolewrite("interrupt: Unknown.\n");
	}
}
