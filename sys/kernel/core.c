#include "core.h"

#include <arch/page.h>
#include <arch/types.h>
#include <config.h>
#include <console.h>
#include <interrupt.h>
#include <panic.h>
#include <process.h>
#include <schedule.h>
#include <string.h>
#include <timer.h>

#include "kvmem.h"
#include "logs.h"
#include "raminit.h"

/* The main function of core 0, in addition to coremain. Returns -1 in case of
   error or 0 otherwise. */
static s8 core0(void);

s8
core0(void)
{
	struct framenode *riframes;

	(void)consolewrite(RAMINIT_LOG_PRE);

	if (raminitframes(&riframes))
		goto panic;

	(void)consolewrite("[");
	(void)consolewriteb16(RAMINIT_BASE);
	(void)consolewrite(" - ");
	(void)consolewriteb16(RAMINIT_BASE + RAMINIT_BINARY_SIZE);
	(void)consolewrite("]\n");

	if (createprocess(riframes, NULL))
		goto panic;

	return 0;

panic:
	tracepanicmsg("core0");
	return -1;
}

void
coremain(u16 c)
{
	if (!c && core0())
		goto panic;

	if (kvmem())
		goto panic;

	enableinterrupts();
	setupnexttimer();

	schedule();

panic:
	tracepanicmsg("coremain");
	return;
}
