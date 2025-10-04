#include "core.h"

#include <arch/types.h>
#include <math.h>
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
		struct framenode *text = NULL, *texttail = NULL;
		uptr a;

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

		/* Save content of sbin/init in the text linked list of
		   frames. */
		for (a = 0; a < tarsize(f->h); a += PAGE_SIZE) {
			struct framenode *fn;

			if (!(fn = palloc(sizeof(struct framenode), 0))) {
				tracepanicmsg("coremain");
				panic();
			}

			fn->f = (void *)(MIN((uptr)tarbase(f->h) + a,
			                     tarsize(f->h)));
			fn->n = NULL;

			/* Append fn to text. */
			if (!texttail)
				text = fn;
			else
				texttail->n = fn;
			texttail = fn;
		}

		/* Create init process. */
		if (createprocess(text, NULL)) {
			tracepanicmsg("coremain");
			panic();
		}
	}

	for (;;);
}
