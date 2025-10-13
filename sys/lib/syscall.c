#include <syscall.h>

#include <arch/types.h>
#include <schedule.h>
#include <trampoline.h>

void
syscall(u16 code, ureg *args)
{
	struct process *p = coreprocess();

	switch (code) {
	case SYSCALL_FORK:
		fork();
		break;
	case SYSCALL_GROWMEM:
		if (!p)
			break;
		setreturn(trapframe(p), (ureg)growmem(args[0]));
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
