#include <arch/types.h>
#include <console.h>
#include <core.h>
#include <driver.h>
#include <interrupt.h>
#include <machine/drivers.h>
#include <spinlock.h>
#include <timer.h>

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
	consolewrite("Core ");
	consolewriteintb10u(c);
	consolewrite(" start.\n");
	releaselock(&l);

	setupnexttimer();

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

	consolewrite("Kernel start.\n");

	physicalmem();

	initdrivers();

	if (freeallmem())
		panic("kernel");

	initdone = 1;

	coremain(0);
}
