#include <interrupt.h>

#include <arch/types.h>
#include <panic.h>

void
interrupt(void)
{
	u8 type = interrupttype();

	switch (type) {
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
