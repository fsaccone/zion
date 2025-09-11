#include <spinlock.h>

#include <atomic.h>

void
acquirelock(struct lock *l)
{
	/* Wait for lock to be unlocked */
	for (;;) {
		if (!atomicswap(&l->locked, 1))
			break;
	}
}

void
releaselock(struct lock *l)
{
	atomicswap(&l->locked, 0);
}
