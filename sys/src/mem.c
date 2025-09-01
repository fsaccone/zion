#include "mem.h"

#include <arch.h>
#include <log.h>
#include <machine.h>
#include <pmem.h>

void
freeallmem(void)
{
	int i;
	uintn freemem[FREE_MEMORY_REGIONS_LEN][2] = { FREE_MEMORY_REGIONS };

	for (i = 0; i < FREE_MEMORY_REGIONS_LEN; i++) {
		uintn start = freemem[i][0],
		      size  = freemem[i][1];

		debugheader("Freeing user memory:");
		debug("From:");
		debugptr((void *)start);
		debug("To:");
		debugptr((void *)(start + size));

		pfreerange((void *)start, (void *)(start + size));
	}
}
