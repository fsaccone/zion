#include <syscall.h>

#include <arch/types.h>

void
syscall(u16 code, ureg *args)
{
	(void)args;

	switch (code) {
	case SYSCALL_SHUTDOWN:
		shutdown();
		break;
	case SYSCALL_REBOOT:
		reboot();
		break;
	default:
	};
}
