#include <interrupt.h>
#include <log.h>

#include "mem.h"

void
kernel(void)
{
	debug("Kernel start.\n");

	physicalmem();

	if (freeallmem())
		panic("kernel");

	setpanicmsg("Reached end of kernel execution.");
	panic("kernel");
}
