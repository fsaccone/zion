#include <log.h>
#include <usermem.h>

void
kmain(void)
{
	debug("Kernel start.");

	usermem_freeall();

	for (;;);
}
