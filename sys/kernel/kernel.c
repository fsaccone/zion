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

/* Initializes serial device driver. */
static void initserialdriver(void);

/* Logs CPU initialization message. */
static void logcpu(void);

static u8 initdone = 0;

void
initserialdriver(void)
{
#ifdef DRIVER_UART
	driver_uart().init();
#endif
}

void
logcpu(void)
{
	(void)consolewrite(CPU_LOG_PRE);
	(void)consolewriteb10u(NCPU);
	(void)consolewrite(" core(s)\n");
}

void
kernel(void)
{
	u16 c = core();

	if (c) {
		while (!initdone);
		coremain(c);
	}

	/* Only core 0 reaches this. */

	initserialdriver();
	logcpu();
	physicalmem();
	initdrivers();
	freeallmem();
	initinterrupts();

	initdone = 1;
	coremain(0);
}
