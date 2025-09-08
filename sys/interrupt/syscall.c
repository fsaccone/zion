#include "syscall.h"

#include <log.h>

void
syscall(u16 type, un args[SYSCALL_ARGS])
{
	int i;

	debug("Syscall of type ");
	debugintbase10u(type);
	debug(" raised. The arguments are:\n");

	for (i = 0; i < SYSCALL_ARGS; i++) {
		debug("- ");
		debugintbase16(args[i]);
		debug("\n");
	}

	setpanicmsg("Unimplemented.");
	panic("syscall");
}
