#include "core.h"

#include <arch/types.h>
#include <panic.h>
#include <pmem.h>
#include <spinlock.h>
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
		struct tarnode *init;

		for (init = initfiles; init; init = init->n) {
			if (!strncmp((char *)init->h->name, "sbin/init",
			             TAR_NAME_SIZE))
				break;
		}

		if (!init) {
			setpanicmsg("Unable to find sbin/init file in "
			            "init.tar.");
			goto panic;
		}

		if (createinitprocess(init->h))
			goto panic;
	}

	for (;;);

panic:
	tracepanicmsg("coremain");
	panic();
}
