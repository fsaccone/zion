#include <interrupt.h>
#include <log.h>

#include "mem.h"

void
kmain(void)
{
	debug("Kernel start.\n");

	physicalmem();

	freeallmem();

	setpanicmsg("Reached end of kernel execution.");
	panic("kmain");
}
