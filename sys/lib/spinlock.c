#include <spinlock.h>

#include <arch/types.h>
#include <atomic.h>
#include <interrupt.h>

void
lock(struct lock *l)
{
	u8 ie = interruptsenabled();

	disableinterrupts();

	/* If first call, set interruptsenabled. */
	if (!l->depth)
		l->interruptsenabled = ie;

	l->depth++;

	/* Wait for lock to be unlocked. */
	for (;;) {
		if (!atomicswap(&l->locked, 1))
			break;
	}
}

void
unlock(struct lock *l)
{
	l->locked = 0;

	l->depth--;

	if (!l->depth && l->interruptsenabled)
		enableinterrupts();
}
