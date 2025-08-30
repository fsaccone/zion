#include <log.h>

#include "mem.h"

void
kmain(void)
{
	debughead("Kernel start.");

	freeallmem();

	panic("Reached end of kernel execution.");
}
