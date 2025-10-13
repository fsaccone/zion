#include <syscall.h>

#include <arch/types.h>

void
syscall(u16 code, ureg *args)
{
	switch (code) {
	case SYSCALL_FORK:
		fork();
		break;
	case SYSCALL_GROWMEM:
		growmem(args[0]);
		break;
	case SYSCALL_SHUTDOWN:
		shutdown();
		break;
	case SYSCALL_REBOOT:
		reboot();
		break;
	default:
	};
}
