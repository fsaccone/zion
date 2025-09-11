#ifndef _SPINLOCK_H
#define _SPINLOCK_H

#include <arch/types.h>

struct lock {
	u8 locked;
};

/* Acquires lock l after waiting for it to be released. */
void acquirelock(struct lock *l);

/* Releases acquired lock l. */
void releaselock(struct lock *l);

#endif
