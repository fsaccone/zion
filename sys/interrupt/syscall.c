#include "syscall.h"

#include <machine/sys.h>
#include <panic.h>
#include <pmem.h>
#include <spinlock.h>

static struct lock l = { 0 };

int
syscall(u16 type, un args[SYSCALL_ARGS])
{
	(void)args;

	switch (type) {
	case SYSCALL_SHUTDOWN:
		/* This lock will never be released. */
		acquirelock(&l);

		pcleanup();

		DO_SYS_SHUTDOWN;

		break;
	case SYSCALL_REBOOT:
		/* This lock will never be released. */
		acquirelock(&l);

		pcleanup();

		DO_SYS_REBOOT;

		break;
	default:
		setpanicmsg("Non-existent system call.");
		tracepanicmsg("syscall");
		return -1;
	};

	return 0;
}
