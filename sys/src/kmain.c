#include <log.h>

#include "mem.h"

void
kmain(void)
{
	debug("Kernel start.\n");

	freeallmem();

	setpanicmsg("Reached end of kernel execution.");
	panic("kmain");
}
