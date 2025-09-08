#include "syscall.h"

#include <log.h>
#include <machine/sys.h>

void
syscall(u16 type, un args[SYSCALL_ARGS])
{
	(void)args;

	switch (type) {
	case SYSCALL_SHUTDOWN:
		DO_SYS_SHUTDOWN;
		return;
	};

	setpanicmsg("Unimplemented.");
	panic("syscall");
}
