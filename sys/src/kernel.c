#include <arch/page.h>
#include <arch/types.h>
#include <config.h>
#include <console.h>
#include <core.h>
#include <interrupt.h>
#include <memswitch.h>
#include <panic.h>
#include <process.h>
#include <schedule.h>
#include <string.h>
#include <timer.h>

#include "mem.h"
#include "kvmem.h"
#include "logs.h"
#include "raminit.h"

/* The main function of core 0. Returns -1 in case of error or 0 otherwise. */
static s8 core0(void);

/* Temporarily one, gets set as the return value of the call to core0. Needed
   to sync cores. */
static s8 core0ret = 1;

static pageentry *ptree = NULL;

s8
core0(void)
{
	struct framenode *riframes;

	initconsole();

	(void)consolewrite(CPU_LOG_PRE);
	(void)consolewriteb10u(NCPU);
	(void)consolewrite(" core(s)\n");

	if (freeallmem())
		goto panic;

	(void)consolewrite(RAMINIT_LOG_PRE);

	if (raminitframes(&riframes))
		goto panic;

	(void)consolewrite("[");
	(void)consolewriteb16(RAMINIT_BASE);
	(void)consolewrite(" - ");
	(void)consolewriteb16(RAMINIT_BASE + RAMINIT_BINARY_SIZE);
	(void)consolewrite("]\n");

	if (createprocess(NULL))
		goto panic;

	if (kvmem(&ptree))
		goto panic;

	return 0;

panic:
	tracepanicmsg("core0");
	return -1;
}

void
kernel(void)
{
	u16 c = core();

	disableinterrupts();

	if (!c && (core0ret = core0()) == -1)
		goto panic;

	while (core0ret);

	disableinterrupts();
	memswitch(ptree);
	enableinterrupts();

	setupnexttimer();

	schedule();

panic:
	tracepanicmsg("kernel");
	panic();
}
