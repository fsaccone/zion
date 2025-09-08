#include <interrupt.h>
#include <log.h>

#include "syscall.h"

void
interrupt(void)
{
	un *args = interruptargs();

	switch (interrupttype()) {
	case INTERRUPT_TYPE_SYSCALL:
		if (syscall((u16)args[0], &args[1]))
			panic("interrupt");
		return;
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
	default:
		setpanicmsg("Unknown.");
	}

	panic("interrupt");
}
