#include <arch/page.h>
#include <arch/types.h>
#include <config.h>
#include <console.h>
#include <core.h>
#include <interrupt.h>
#include <machine/mem.h>
#include <memswitch.h>
#include <panic.h>
#include <pmem.h>
#include <process.h>
#include <schedule.h>
#include <string.h>
#include <timer.h>
#include <vmem.h>

#include "mem.h"
#include "kvmem.h"
#include "logs.h"

/* The main function of core 0. Returns -1 in case of error or 0 otherwise. */
static s8 core0(void);

/* Temporarily one, gets set as the return value of the call to core0. Needed
   to sync cores. */
static s8 core0ret = 1;

static pageentry *ptree = NULL;

s8
core0(void)
{
	struct process *rip;
	uptr ria;
	struct pageoptions riopts = {
		.u = 1,
		.r = 1,
		.w = 0,
		.x = 1,
		.a = 0,
	};

	initconsole();

	(void)consolewrite(CPU_LOG_PRE);
	(void)consolewriteb10u(NCPU);
	(void)consolewrite(" core(s)\n");

	if (freeallmem())
		goto panic;

	if (kvmem(&ptree))
		goto panic;

	if (!(rip = allocprocess(NULL)))
		goto panic;

	(void)consolewrite(RAMINIT_LOG_PRE);

	for (ria = RAMINIT_START; ria < RAMINIT_END; ria += PAGE_SIZE) {
		if (growprocess(NULL, rip, (void *)ria, riopts))
			goto panic;
	}

	(void)consolewrite("[");
	(void)consolewriteb16(RAMINIT_START);
	(void)consolewrite(" - ");
	(void)consolewriteb16(RAMINIT_END);
	(void)consolewrite("]\n");

	rip->state = READY;

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
