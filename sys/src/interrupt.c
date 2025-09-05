#include <arch/types.h>
#include <interrupt.h>
#include <log.h>

void
interrupt(u8 type)
{
	switch (type) {
	case INTERRUPT_TYPE_UNKNOWN:
		panic("Unknown interrupt.");
		break;
	case INTERRUPT_TYPE_EXCEPTION:
		panic("Exception.");
		break;
	case INTERRUPT_TYPE_HARDWARE:
		panic("Hardware interrupt.");
		break;
	case INTERRUPT_TYPE_SOFTWARE:
		panic("Software interrupt.");
		break;
	case INTERRUPT_TYPE_TIMER:
		panic("Timer interrupt.");
		break;
	case INTERRUPT_TYPE_SYSCALL:
		panic("Syscall.");
		break;
	}
}
