#include <config.h>
#include <driver.h>
#include <interrupt.h>
#include <log.h>

#include "mem.h"

void
kernel(void)
{
	debug("Kernel start.\n");

	physicalmem();

#ifdef CONFIG_DRIVER_UART
	driver_uart().init();
#endif

	if (freeallmem())
		panic("kernel");

	setpanicmsg("Reached end of kernel execution.");
	panic("kernel");
}
