#ifndef _SPINLOCK_H
#define _SPINLOCK_H

#include <arch/types.h>

struct lock {
	/* If 0, lock is not acquired; otherwise, it is. */
	un locked;

	/* The difference of acquirelock and releaselock calls on this lock. */
	u8 depth;

	/* The state of interrupts at the first acquirelock call on this lock.
	   If 0, interrupts were disabled; otherwise, they were enabled. */
	u8 interruptsenabled;
};

/* Acquires lock l after waiting for it to be released. */
void acquirelock(struct lock *l);

/* Releases acquired lock l. */
void releaselock(struct lock *l);

#endif
