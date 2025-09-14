#include "user.h"

#include <interrupt.h>
#include <panic.h>

#include "syscall.h"

void
userinterrupt(un args[INTERRUPT_ARGS])
{
	switch (interrupttype()) {
	case INTERRUPT_TYPE_SYSCALL:
		if (syscall((u16)args[0], &args[1])) {
			tracepanicmsg("userinterrupt");
			panic();
		}

		break;
	default:
	}
}
