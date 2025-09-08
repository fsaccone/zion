#include <interrupt.h>
#include <log.h>

void
interrupt(void)
{
	switch (getinterrupttype()) {
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
		setpanicmsg("Syscall.");
		break;
	case INTERRUPT_TYPE_UNKNOWN:
	default:
		setpanicmsg("Unknown.");
	}

	panic("interrupt");
}
