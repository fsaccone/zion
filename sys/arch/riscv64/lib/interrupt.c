#include <core.h>
#include <driver.h>
#include <interrupt.h>
#include <machine/mem.h>

struct driver
interruptdriver(void)
{
	struct driver null = { 0 };

	if (interrupttype() != INTERRUPT_TYPE_HARDWARE)
		return null;

	switch (*(u32 *)((un)PLIC + 0x2004 + core() * 0x2000)) {
	case UART0_IRQ:
		return driver_uart();
	}

	return null;
}
