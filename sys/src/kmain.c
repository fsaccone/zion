#include <log.h>

#include "mem.h"

void
kmain(void)
{
	debugheader("Kernel start.");

	freeallmem();

	panic("Reached end of kernel execution.");
}
