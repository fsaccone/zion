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
		debug("interrupt: Exception.\n");
		break;
	case INTERRUPT_TYPE_HARDWARE:
		debug("interrupt: Hardware.\n");
		break;
	case INTERRUPT_TYPE_SOFTWARE:
		debug("interrupt: Software.\n");
		break;
	case INTERRUPT_TYPE_TIMER:
		debug("interrupt: Timer.\n");
		break;
	default:
		debug("interrupt: Unknown.\n");
	}
}
