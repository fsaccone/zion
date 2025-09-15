#include <core.h>
#include <driver.h>
#include <interrupt.h>
#include <machine/mem.h>

#define PLIC_MEM_CLAIM(c) ((uptr)PLIC + 0x2004 + (c) * 0x2000)

u8
interruptdriver(struct driver *drv)
{
	if (interrupttype() != INTERRUPT_TYPE_HARDWARE)
		return -1;

	switch (*(u32 *)PLIC_MEM_CLAIM(core())) {
	case UART0_IRQ:
		*drv = driver_uart();
		return 0;
	default:
	}

	return -1;
}
