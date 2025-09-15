#ifndef _SPINLOCK_H
#define _SPINLOCK_H

#include <arch/types.h>

struct lock {
	/* If 0, lock is not acquired; otherwise, it is. This needs to be the
	   size of a register because the atomic swap generally works with
	   values of this size. */
	ureg locked;

	/* The difference of lock and unlock calls on this lock. */
	u8 depth;

	/* The state of interrupts at the first lock call on this lock. If 0,
	   interrupts were disabled; otherwise, they were enabled. */
	u8 interruptsenabled;
};

/* Acquires lock l after waiting for it to be released. */
void lock(struct lock *l);

/* Releases acquired lock l. */
void unlock(struct lock *l);

#endif
