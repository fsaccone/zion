#include <arch/types.h>
#include <console.h>
#include <core.h>
#include <driver.h>
#include <machine/cpu.h>
#include <machine/drivers.h>
#include <panic.h>

#include "core.h"
#include "drivers.h"
#include "logs.h"
#include "mem.h"

static u8 initdone = 0;

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

	(void)consolewrite(CPU_LOG_PRE);
	(void)consolewriteintb10u(NCPU);
	(void)consolewrite(" core(s)\n");

	physicalmem();

	initdrivers();

	if (freeallmem()) {
		tracepanicmsg("kernel");
		panic();
	}

	initdone = 1;

	coremain(0);
}
