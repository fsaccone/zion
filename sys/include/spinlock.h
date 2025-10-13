#ifndef _SPINLOCK_H
#define _SPINLOCK_H

#include <arch/types.h>
#include <machine/cpu.h>

/* The spinlock. */
struct lock {
	/* The state of the lock. It is 0 when the lock is not acquired. The
	   size of this is the size of a register to make the atomic swap
	   possible. */
	ureg locked;

	/* The amount of times the lock was acquired for each core. */
	u8 depth[NCPU];

	/* The state of interrupts during the first acquiring of this lock for
	   each core: 0 if disabled, 1 otherwise. */
	u8 interruptsenabled[NCPU];
};

/* If the l lock is acquired by the calling core, 1 is returned. Otherwise, 0
   is returned. */
u8 holding(struct lock *l);

/* It acquires lock l after waiting for it to be released. */
void lock(struct lock *l);

/* It releases acquired lock l. */
void unlock(struct lock *l);

#endif
