#include <pagefault.h>

#include <arch/types.h>
#include <panic.h>

void
pagefault(u8 u)
{
	if (u)
		setpanicmsg("User page fault.");
	else
		setpanicmsg("Kernel page fault.");

	tracepanicmsg("pagefault");
	panic();
}
