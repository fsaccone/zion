#include <config.h>
#include <driver.h>
#include <interrupt.h>
#include <log.h>
#include <timer.h>

#include "drivers.h"
#include "mem.h"

void
kernel(void)
{
	debug("Kernel start.\n");

	physicalmem();
	inittimer();

	initdrivers();

	if (freeallmem())
		panic("kernel");

	setpanicmsg("Reached end of kernel execution.");
	panic("kernel");
}
