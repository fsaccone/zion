#include <handleint.h>

#include <arch/types.h>
#include <console.h>
#include <interrupt.h>
#include <panic.h>
#include <schedule.h>
#include <syscall.h>
#include <timer.h>

#include "syscall/calls.h"

/* The first interrupt argument is used to identify the type of the syscall. */
#define SYSCALL_ARGS (INTERRUPT_ARGS - 1)

/* Handle system call of type type and arguments args. */
static void syscall(u16 type, ureg args[SYSCALL_ARGS]);

void
syscall(u16 type, ureg args[SYSCALL_ARGS])
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

void
handleint(void)
{
	ureg *args = interruptargs();
	u8 isuser = interruptisuser(),
	   type   = interrupttype();

	switch (type) {
	case INTERRUPT_TYPE_SYSCALL:
		syscall((u16)args[0], &args[1]);

		break;
	case INTERRUPT_TYPE_EXCEPTION:
		if (isuser)
			break;

		setpanicmsg("Exception.");
		goto panic;

		break;
	case INTERRUPT_TYPE_HARDWARE:
		(void)consolewrite("Hardware interrupt.\n");

		break;
	case INTERRUPT_TYPE_TIMER:
		setupnexttimer();
		nextschedule();

		break;
	default:
	}

	return;

panic:
	tracepanicmsg("handleint");
	panic();
}
