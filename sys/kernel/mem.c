#include "mem.h"

#include <arch/types.h>
#include <log.h>
#include <machine/mem.h>
#include <pmem.h>
#include <vmem.h>

int
freeallmem(void)
{
	int i;
	un freemem[FREE_MEMORY_REGIONS_LEN][2] = { FREE_MEMORY_REGIONS };

	for (i = 0; i < FREE_MEMORY_REGIONS_LEN; i++) {
		un start = freemem[i][0],
		   size  = freemem[i][1];

		debug("Freeing user memory going from ");
		debugintbase16(start);
		debug(" to ");
		debugintbase16((start + size));
		debug(".\n");

		if (pfreerange((void *)start, (void *)(start + size))) {
			tracepanicmsg("freeallmem");
			return -1;
		}
	}

	return 0;
}

void
physicalmem(void)
{
	memswitch(NULL, 0);
}
