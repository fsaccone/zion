#ifndef _DRIVER_H
#define _DRIVER_H

struct driver {
	void (*init)(void);
};

/* A list of drivers follows: each is a driver_NAME function with no parameters
   which returns the corresponding struct driver. */

struct driver driver_uart(void);

#endif
