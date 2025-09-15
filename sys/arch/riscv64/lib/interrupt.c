#include <core.h>
#include <driver.h>
#include <interrupt.h>
#include <machine/mem.h>

u8
interruptdriver(struct driver *drv)
{
	if (interrupttype() != INTERRUPT_TYPE_HARDWARE)
		return -1;

	switch (*(u32 *)((uptr)PLIC + 0x2004 + core() * 0x2000)) {
	case UART0_IRQ:
		*drv = driver_uart();
		return 0;
	default:
	}

	return -1;
}
