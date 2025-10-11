#include <exception.h>

#include <arch/types.h>
#include <panic.h>

void
exception(u8 u)
{
	if (u)
		setpanicmsg("User exception.");
	else
		setpanicmsg("Kernel exception.");

	tracepanicmsg("exception");
	panic();
}
