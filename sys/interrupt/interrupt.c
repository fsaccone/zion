#include <console.h>
#include <driver.h>
#include <interrupt.h>
#include <panic.h>
#include <timer.h>

#include "syscall.h"

void
interrupt(void)
{
	ureg *args   = interruptargs();
	u8    isuser = interruptisuser(),
	      type   = interrupttype();
	struct driver drv;

	switch (type) {
	case INTERRUPT_TYPE_SYSCALL:
		if (syscall((u16)args[0], &args[1])) {
			tracepanicmsg("interrupt");
			panic();
		}

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
