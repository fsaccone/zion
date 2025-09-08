#include <spinlock.h>

#include <atomic.h>
#include <interrupt.h>
#include <log.h>

int
acquirelock(struct lock *l)
{
	if (l->locked) {
		setpanicmsg("Lock already acquired.");
		tracepanicmsg("acquirelock");
		return -1;
	}

	l->intenabled = interruptsenabled();

	if (l->intenabled)
		disableinterrupts();

	/* Wait for lock to be unlocked */
	while (atomicswap(&l->locked, 1));

	return 0;
}

int
releaselock(struct lock *l)
{
	if (!l->locked) {
		setpanicmsg("Lock already released.");
		tracepanicmsg("releaselock");
		return -1;
	}

	atomicswap(&l->locked, 0);

	if (l->intenabled)
		enableinterrupts();

	return 0;
}
