#include <syscall.h>

#include <arch/types.h>

#include "syscall/calls.h"

void
syscall(u16 type, ureg *args)
{
	(void)args;

	switch (type) {
	case SYSCALL_SHUTDOWN:
		shutdown();
		break;
	case SYSCALL_REBOOT:
		reboot();
		break;
	default:
	};
}
