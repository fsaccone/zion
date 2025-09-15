#include <core.h>
#include <driver.h>
#include <interrupt.h>
#include <machine/mem.h>

#define PLIC_MEM_CLAIM(c)     ((uptr)PLIC + 0x2004 + (c) * 0x2000)
#define PLIC_MEM_ENABLE(c)    ((uptr)PLIC + 0x2000 + (c) * 0x80)
#define PLIC_MEM_PRIORITY(id) ((uptr)PLIC + (id) * 0x4)
#define PLIC_MEM_THRESHOLD(c) ((uptr)PLIC + 0x200004 + (c) * 0x1000)

void
initinterrupts(void)
{
	/* While the main initialization (e.g. mie, mstatus, ...) has been done
	   by astart, PLIC is still uninitialized. */

	u16 c = core();

	*(u32 *)PLIC_MEM_PRIORITY(UART0_IRQ) = UART0_PRIORITY;

	/* Enable devices. */
	*(u32 *)PLIC_MEM_ENABLE(c) = (1 << UART0_IRQ);

	*(u32 *)PLIC_MEM_THRESHOLD(c) = 0;
}

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
