#include <arch/types.h>
#include <console.h>
#include <core.h>
#include <interrupt.h>
#include <machine/cpu.h>
#include <panic.h>

#include "core.h"
#include "logs.h"
#include "mem.h"

static u8 initdone = 0;

void
kernel(void)
{
	u16 c = core();

	disableinterrupts();

	if (c) {
		while (!initdone);

		coremain(c);

		/* If coremain returns. */
		goto panic;
	}

	/* Only core 0 reaches this. */

	initconsole();

	(void)consolewrite(CPU_LOG_PRE);
	(void)consolewriteb10u(NCPU);
	(void)consolewrite(" core(s)\n");

	if (freeallmem())
		goto panic;

	initdone = 1;
	coremain(0);

	/* If coremain returns. */
panic:
	tracepanicmsg("kernel");
	panic();
}
