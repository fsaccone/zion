#include <arch/types.h>
#include <console.h>
#include <core.h>
#include <driver.h>
#include <interrupt.h>
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

	if (c) {
		while (!initdone);
		coremain(c);
	}

	/* Only core 0 reaches this. */

	initconsole();

	(void)consolewrite(CPU_LOG_PRE);
	(void)consolewriteb10u(NCPU);
	(void)consolewrite(" core(s)\n");

	if (freeallmem()) {
		tracepanicmsg("kernel");
		panic();
	}

	initdrivers();
	initinterrupts();

	initdone = 1;
	coremain(0);
}
