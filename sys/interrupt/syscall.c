#include "syscall.h"

#include <arch/types.h>
#include <machine/sys.h>
#include <panic.h>
#include <pmem.h>
#include <spinlock.h>

static struct lock l = { 0 };

void
syscall(u16 type, ureg args[SYSCALL_ARGS])
{
	(void)args;

	switch (type) {
	case SYSCALL_SHUTDOWN:
		/* This lock will never be released. */
		lock(&l);

		pcleanup();

		DO_SYS_SHUTDOWN;

		break;
	case SYSCALL_REBOOT:
		/* This lock will never be released. */
		lock(&l);

		pcleanup();

		DO_SYS_REBOOT;

		break;
	default:
	};
}
