#include <machine/drivers.h>

#ifdef DRIVER_PCI

#include <driver.h>

#include <pmem.h>

static void init(void);

struct driver
driver_pci(void)
{
	struct driver d = {
		.init        = init,
		.interrupt   = NULL,
		.write.block = NULL,
	};

	return d;
};

void
init(void)
{
}

#endif
