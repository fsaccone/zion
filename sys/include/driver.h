#ifndef _DRIVER_H
#define _DRIVER_H

struct driver {
	void (*init)(void);
};

#endif
