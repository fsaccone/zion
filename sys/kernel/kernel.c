#include <arch/types.h>
#include <config.h>
#include <core.h>
#include <driver.h>
#include <interrupt.h>
#include <log.h>
#include <spinlock.h>

#include "drivers.h"
#include "mem.h"

static u8 initdone = 0;
static struct lock l = { 0 };

void
kernel(void)
{
	u16 c = core();

	/* For every core except 0 (main core). */
	if (c) {
		while (!initdone);

		acquirelock(&l);
		debug("Core ");
		debugintbase10u(c);
		debug(" started.\n");
		releaselock(&l);

		for (;;);
	}

#ifdef CONFIG_DRIVER_UART
	driver_uart().init();
#endif

	debug("Kernel start.\n");

	physicalmem();

	initdrivers();

	if (freeallmem())
		panic("kernel");

	initdone = 1;

	for (;;);
}
