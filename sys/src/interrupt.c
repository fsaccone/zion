#include <log.h>

void
interrupt(void)
{
	setpanicmsg("Unimplemented");
	debug("interrupt");
}
