#include "user.h"

#include <interrupt.h>
#include <panic.h>

void
userinterrupt(un args[INTERRUPT_ARGS])
{
	(void)args;

	setpanicmsg("Interrupt caused by user mode code.");
	tracepanicmsg("userinterruptmsg");
	panic();
}
