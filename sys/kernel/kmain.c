#include <arch.h>
#include <log.h>
#include <machine.h>
#include <pmem.h>

void
kmain(void)
{
	int i, *p, *q;
	uintn_t freemem[FREE_MEMORY_REGIONS_LEN][2] = { FREE_MEMORY_REGIONS };

	debug("Kernel start.");

	for (i = 0; i < FREE_MEMORY_REGIONS_LEN; i++) {
		uintn_t start = freemem[i][0],
		        size  = freemem[i][1];

		debug("Freeing user memory:");
		debug("  From:");
		debugptr((void *)start);
		debug("  To:");
		debugptr((void *)(start + size));

		pfreerange((void *)start, (void *)(start + size));
	}

	for (;;);
}
