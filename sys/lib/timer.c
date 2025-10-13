#include <timer.h>

#include <process.h>
#include <schedule.h>
#include <spinlock.h>

void
timer(void)
{
	struct process *p;

	setupnexttimer();

	if ((p = coreprocess())) {
		lock(&p->lock);
		nextschedule();
	}
}
