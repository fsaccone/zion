#include "core.h"

#include <arch/types.h>
#include <panic.h>
#include <pmem.h>
#include <process.h>
#include <spinlock.h>
#include <string.h>
#include <tar.h>
#include <timer.h>

#include "inittar.h"

static struct tarnode *initfiles = NULL;
static struct lock     l         = { 0 };

void
coremain(u16 c)
{
	setupnexttimer();

	lock(&l);
	if (!initfiles && !(initfiles = allocinittarfiles())) {
		setpanicmsg("Unable to find init.tar in kernel codespace.");
		tracepanicmsg("coremain");
		panic();
	}
	unlock(&l);

	if (!c) {
		struct tarnode *f;
		void *initbase;

		for (f = initfiles; f; f = f->n) {
			if (!strncmp((char *)f->h->name, "sbin/init",
			             TAR_NAME_SIZE))
				break;
		}

		if (!f) {
			setpanicmsg("Unable to find sbin/init file in "
			            "init.tar.");
			tracepanicmsg("coremain");
			panic();
		}

		/* Copy content of sbin/init aligning it to PAGE_SIZE. */
		if (!(initbase = palloc(tarsize(f->h), 0))) {
			tracepanicmsg("coremain");
			panic();
		}
		pmemcpy(initbase, tarbase(f->h), tarsize(f->h));

		/* Allocate init process. */
		if (allocprocess(initbase, tarsize(f->h), NULL)) {
			tracepanicmsg("coremain");
			panic();
		}
	}

	for (;;);
}
