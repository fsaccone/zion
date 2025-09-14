#include "mem.h"

#include <arch/types.h>
#include <console.h>
#include <machine/mem.h>
#include <panic.h>
#include <pmem.h>
#include <vmem.h>

#include "logs.h"

int
freeallmem(void)
{
	int i;
	un freemem[FREE_MEMORY_REGIONS_LEN][2] = { FREE_MEMORY_REGIONS };

	for (i = 0; i < FREE_MEMORY_REGIONS_LEN; i++) {
		un start = freemem[i][0],
		   size  = freemem[i][1];

		consolewrite(MEM_LOAD_LOG_PRE);
		consolewrite("From ");
		consolewriteintb16(start);
		consolewrite(" to ");
		consolewriteintb16((start + size));
		consolewrite(".\n");

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
