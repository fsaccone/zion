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

static struct tarnode *initfiles = NULL;
static struct lock     l         = { 0 };

void
coremain(u16 c)
{
	setupnexttimer();

	lock(&l);
	if (!initfiles && !(initfiles = allocinittarfiles()))
		goto panic;
	unlock(&l);

	if (!c) {
		struct tarheader *init;

		if (!(init = findinitfile(initfiles))) {
			setpanicmsg("Unable to find sbin/init file in "
			            "init.tar.");
			goto panic;
		}

		if (createinitprocess(init))
			goto panic;
	}

	schedule();

panic:
	tracepanicmsg("coremain");
	panic();
}
