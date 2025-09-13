#include <arch/types.h>
#include <core.h>
#include <driver.h>
#include <interrupt.h>
#include <log.h>
#include <machine/drivers.h>
#include <spinlock.h>

#include "drivers.h"
#include "mem.h"

/* The main function of every CPU core with core ID c. */
static void coremain(u16 c);

static u8          initdone = 0;
static struct lock l        = { 0 };

void
coremain(u16 c)
{
	acquirelock(&l);
	debug("Core ");
	debugintbase10u(c);
	debug(" started.\n");
	releaselock(&l);

	for (;;);
}

void
kernel(void)
{
	u16 c = core();

	/* For every core except 0 (main core). */
	if (c) {
		while (!initdone);
		coremain(c);
	}

#ifdef DRIVER_UART
	driver_uart().init();
#endif

	debug("Kernel start.\n");

	physicalmem();

	initdrivers();

	if (freeallmem())
		panic("kernel");

	initdone = 1;

	coremain(0);
}
