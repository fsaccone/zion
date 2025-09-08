#include "syscall.h"

#include <log.h>
#include <machine/sys.h>
#include <pmem.h>

int
syscall(u16 type, un args[SYSCALL_ARGS])
{
	(void)args;

	switch (type) {
	case SYSCALL_SHUTDOWN:
		pcleanup();
		DO_SYS_SHUTDOWN;
		break;
	case SYSCALL_REBOOT:
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
