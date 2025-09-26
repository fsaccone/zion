#include "drivers.h"

#include <driver.h>
#include <machine/drivers.h>

void
initdrivers(void)
{
#ifdef DRIVER_PCI
	driver_pci().init();
#endif
}
