#include <interrupt.h>
#include <log.h>
#include <types.h>

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
		panic("TIMER interrupt.");
		break;
	}
}
