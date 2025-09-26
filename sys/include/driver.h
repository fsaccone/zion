#ifndef _DRIVER_H
#define _DRIVER_H

#include <arch/types.h>
#include <machine/drivers.h>

struct driver {
	/* Initializes the driver. */
	void (*init)(void);

	/* Called by the interrupt handler in case of a driver interrupt from
	   this device. May be NULL. */
	void (*interrupt)(void);

	/* In case of a character device, write character c to device; in case
	   of a block device, write n bytes starting from c to the device. May
	   be NULL */
	union {
		void (*block)(u8 *c, uptr n);
		void (*character)(u8 c);
	} write;
};

/* A list of drivers follows: each is a driver_NAME function with no parameters
   which returns the corresponding struct driver. */

#ifdef DRIVER_PCI
struct driver driver_pci(void);
#endif
#ifdef DRIVER_UART
struct driver driver_uart(void);
#endif

#endif
