#include <spinlock.h>

#include <arch/types.h>
#include <atomic.h>
#include <core.h>
#include <interrupt.h>

void
lock(struct lock *l)
{
	u8 ie = interruptsenabled();
	u16 c = core();

	disableinterrupts();

	/* If this is the first call, set interruptsenabled. */
	if (!l->depth[c])
		l->interruptsenabled[c] = ie;

	l->depth[c]++;

	/* Wait for the lock to be unlocked. */
	while (atomicswap(&l->locked, 1));
}

void
unlock(struct lock *l)
{
	u16 c = core();

	l->locked = 0;

	l->depth[c]--;

	if (!l->depth[c] && l->interruptsenabled[c])
		enableinterrupts();
}
