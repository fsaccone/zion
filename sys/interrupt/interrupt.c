#include <interrupt.h>
#include <panic.h>

#include "kernel.h"
#include "syscall.h"
#include "user.h"

void
interrupt(void)
{
	ureg *args   = interruptargs();
	u8    isuser = interruptisuser(),
	      type   = interrupttype();

	switch (type) {
	case INTERRUPT_TYPE_SYSCALL:
		if (syscall((u16)args[0], &args[1])) {
			tracepanicmsg("userinterrupt");
			panic();
		}

		break;
	default:
	}

	if (isuser)
		userinterrupt(args);
	else
		kernelinterrupt();
}
