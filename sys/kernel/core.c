#include "core.h"

#include <arch/types.h>
#include <panic.h>
#include <pmem.h>
#include <spinlock.h>
#include <schedule.h>
#include <string.h>
#include <tar.h>
#include <timer.h>

#include "init.h"
#include "inittar.h"

/* The main function of core 0, in addition to coremain. Returns -1 in case of
   error or 0 otherwise. */
static s8 core0(void);

static struct tarnode *initfiles = NULL;
static struct lock     l         = { 0 };

s8
core0(void)
{
	struct tarheader *init;

	if (!(init = findinitfile(initfiles))) {
		setpanicmsg("Unable to find sbin/init file in "
		            "init.tar.");
		goto panic;
	}

	if (createinitprocess(init))
		goto panic;

	return 0;

panic:
	tracepanicmsg("core0");
	return -1;
}

void
coremain(u16 c)
{
	setupnexttimer();

	lock(&l);
	if (!initfiles && !(initfiles = allocinittarfiles()))
		goto panic;
	unlock(&l);

	if (!c && core0())
		goto panic;

	schedule();

panic:
	tracepanicmsg("coremain");
	panic();
}
