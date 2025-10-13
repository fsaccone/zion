#include <syscall.h>

#include <machine/sys.h>
#include <memswitch.h>
#include <pmem.h>
#include <spinlock.h>

static struct lock l = { 0 };

void
reboot(void)
{
	/* This lock is never released. */
	lock(&l);

	pcleanup();
	pmemswitch();
	DO_SYS_REBOOT;
}
