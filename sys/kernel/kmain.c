#include <log.h>
#include <usermem.h>

void
kmain(void *kstart)
{
	debug("Kernel start.");

	usermem_init(kstart);

	for (;;);
}
