#ifndef _DRIVER_H
#define _DRIVER_H

#include <arch/types.h>

struct driver {
	/* Initializes the driver. */
	void (*init)(void);

	/* Writes n bytes of content c to the driver. May be undefined. */
	void (*write)(u8 *c, u32 n);
};

/* A list of drivers follows: each is a driver_NAME function with no parameters
   which returns the corresponding struct driver. */

struct driver driver_uart(void);

#endif
