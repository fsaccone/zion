#include <arch/types.h>
#include <interrupt.h>
#include <log.h>

void
interrupt(u8 type)
{
	switch (type) {
	case INTERRUPT_TYPE_EXCEPTION:
		panic("Exception.\n");
		break;
	case INTERRUPT_TYPE_HARDWARE:
		panic("Hardware interrupt.\n");
		break;
	case INTERRUPT_TYPE_SOFTWARE:
		panic("Software interrupt.\n");
		break;
	case INTERRUPT_TYPE_TIMER:
		panic("Timer interrupt.\n");
		break;
	case INTERRUPT_TYPE_SYSCALL:
		panic("Syscall.\n");
		break;
	case INTERRUPT_TYPE_UNKNOWN:
	default:
		panic("Unknown interrupt.\n");
	}
}
