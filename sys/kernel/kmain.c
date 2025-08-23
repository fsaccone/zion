#include <log.h>

void
kmain(void *kstart)
{
	(void)kstart;

	debug("Kernel start.");

	for (;;);
}
