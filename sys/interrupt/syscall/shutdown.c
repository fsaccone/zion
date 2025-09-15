#include "calls.h"

#include <machine/sys.h>
#include <pmem.h>
#include <spinlock.h>

static struct lock l = { 0 };

void
shutdown(void)
{
	/* This lock will never be released. */
	lock(&l);

	pcleanup();
	DO_SYS_SHUTDOWN;
}
