#include <log.h>

void
interrupt(void)
{
	setpanicmsg("Unimplemented");
	panic("interrupt");
}
