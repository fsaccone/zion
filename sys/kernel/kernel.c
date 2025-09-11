#include <config.h>
#include <driver.h>
#include <interrupt.h>
#include <log.h>

#include "drivers.h"
#include "mem.h"

void
kernel(void)
{
#ifdef CONFIG_DRIVER_UART
	driver_uart().init();
#endif

	debug("Kernel start.\n");

	physicalmem();

	initdrivers();

	if (freeallmem())
		panic("kernel");

	setpanicmsg("Reached end of kernel execution.");
	panic("kernel");
}
