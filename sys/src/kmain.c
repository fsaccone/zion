#include <interrupt.h>
#include <log.h>

#include "interrupt.h"
#include "mem.h"

void
kmain(void)
{
	debug("Kernel start.\n");

	setinterrupthandler(interrupt);

	freeallmem();

	setpanicmsg("Reached end of kernel execution.");
	panic("kmain");
}
