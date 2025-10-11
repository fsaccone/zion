#include <exception.h>

#include <panic.h>

void
exception(void)
{
	setpanicmsg("Exception.");
	tracepanicmsg("exception");
	panic();
}
