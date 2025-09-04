#include <spinlock.h>

#include <atomic.h>
#include <interrupt.h>
#include <log.h>

void
acquirelock(struct lock *l)
{
	if (l->locked)
		panic("acquirelock: Lock already acquired.");

	l->intenabled = interruptsenabled();

	if (l->intenabled)
		disableinterrupts();

	/* Wait for lock to be unlocked */
	while (atomicswap(&l->locked, 1));
}

void
releaselock(struct lock *l)
{
	if (!l->locked)
		panic("releaselock: Lock already released.");

	atomicswap(&l->locked, 0);

	if (l->intenabled)
		enableinterrupts();
}
