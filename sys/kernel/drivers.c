#include "drivers.h"

#include <driver.h>

void
initdrivers(void)
{
#ifdef CONFIG_DRIVER_UART
	driver_uart().init();
#endif
}
