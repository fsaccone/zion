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
		syscall((u16)args[0], &args[1]);

		break;
	case INTERRUPT_TYPE_EXCEPTION:
		if (isuser)
			break;

		setpanicmsg("Exception.");
		tracepanicmsg("interrupt");
		panic();

		break;
	case INTERRUPT_TYPE_HARDWARE:
		if (!interruptdriver(&drv) && drv.interrupt)
			drv.interrupt();

		break;
	case INTERRUPT_TYPE_TIMER:
		setupnexttimer();

		break;
	default:
	}
}
