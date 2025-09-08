#include <interrupt.h>
#include <log.h>

#include "syscall.h"

void
interrupt(void)
{
	un *args = interruptargs();

	switch (interrupttype()) {
	case INTERRUPT_TYPE_EXCEPTION:
		setpanicmsg("Exception.");
		break;
	case INTERRUPT_TYPE_HARDWARE:
		setpanicmsg("Hardware.");
		break;
	case INTERRUPT_TYPE_SOFTWARE:
		setpanicmsg("Software.");
		break;
	case INTERRUPT_TYPE_TIMER:
		setpanicmsg("Timer.");
		break;
	case INTERRUPT_TYPE_SYSCALL:
		syscall((u8)args[0], &args[1]);
		return;
	default:
		setpanicmsg("Unknown.");
	}

	panic("interrupt");
}
