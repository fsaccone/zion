#include <console.h>
#include <driver.h>
#include <interrupt.h>
#include <panic.h>
#include <timer.h>

#include "syscall.h"

void
kernelinterrupt(void)
{
	struct driver drv;

	switch (interrupttype()) {
	case INTERRUPT_TYPE_SYSCALL:
		setpanicmsg("System call caused by kernel mode code.");
		tracepanicmsg("kernelinterrupt");
		panic();

		break;
	case INTERRUPT_TYPE_EXCEPTION:
		setpanicmsg("Exception.");
		tracepanicmsg("interrupt");
		panic();

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
