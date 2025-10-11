#include <timer.h>

#include <schedule.h>

void
timer(void)
{
	setupnexttimer();
	nextschedule();
}
