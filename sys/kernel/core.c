#include "core.h"

#include <arch/types.h>
#include <panic.h>
#include <pmem.h>
#include <spinlock.h>
#include <timer.h>

#include "inittar.h"

static struct tarnode *initfiles = NULL;
static struct lock     l         = { 0 };

void
coremain(u16 c)
{
	(void)c;

	setupnexttimer();

	lock(&l);
	if (!initfiles && !(initfiles = allocinittarfiles())) {
		setpanicmsg("init.tar was not found in kernel codespace.");
		tracepanicmsg("coremain");
		panic();
	}
	unlock(&l);

	for (;;);
}
