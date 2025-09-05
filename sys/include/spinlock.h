#ifndef _SPINLOCK_H
#define _SPINLOCK_H

#include <arch/types.h>

struct lock {
	un locked;
	u8 intenabled;
};

/* Acquires lock l after waiting for it to be released. Returns -1 in case of
   error and 0 otherwise. */
int acquirelock(struct lock *l);

/* Releases acquired lock l. Returns -1 in case of error and 0 otherwise. */
int releaselock(struct lock *l);

#endif
