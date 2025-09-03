#ifndef _SPINLOCK_H
#define _SPINLOCK_H

#include <arch.h>

struct lock {
	uintn locked;
	uint8 intenabled;
};

/* Acquires lock l after waiting for it to be released */
void acquirelock(struct lock *l);

/* Releases acquired lock l */
void releaselock(struct lock *l);

#endif
