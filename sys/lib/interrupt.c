#include <interrupt.h>

#include <arch/types.h>
#include <console.h>
#include <panic.h>
#include <schedule.h>
#include <syscall.h>
#include <timer.h>

void
interrupt(void)
{
	u8 type = interrupttype();

	switch (type) {
	case INTERRUPT_TYPE_HARDWARE:
		(void)consolewrite("Hardware interrupt.\n");

		break;
	case INTERRUPT_TYPE_TIMER:
		setupnexttimer();
		nextschedule();

		break;
	case INTERRUPT_TYPE_PAGE_FAULT:
		setpanicmsg("Page fault.");
		goto panic;

		break;
	default:
	}

	return;

panic:
	tracepanicmsg("handleinterrupt");
	panic();
}
