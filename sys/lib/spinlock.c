#include <spinlock.h>

#include <atomic.h>
#include <interrupt.h>
#include <log.h>

void
acquirelock(struct lock *l)
{
	/* Wait for lock to be unlocked */
	while (atomicswap(&l->locked, 1));
}

void
releaselock(struct lock *l)
{
	atomicswap(&l->locked, 0);
}
