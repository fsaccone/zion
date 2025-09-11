#include <spinlock.h>

#include <atomic.h>
#include <interrupt.h>
#include <log.h>

int
acquirelock(struct lock *l)
{
	/* Wait for lock to be unlocked */
	while (atomicswap(&l->locked, 1));

	return 0;
}

int
releaselock(struct lock *l)
{
	atomicswap(&l->locked, 0);

	return 0;
}
