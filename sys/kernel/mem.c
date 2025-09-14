#include "mem.h"

#include <arch/page.h>
#include <arch/types.h>
#include <console.h>
#include <machine/mem.h>
#include <math.h>
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
		   size  = freemem[i][1],
		   done  = 0;
		void *f;

		for (f = (void *)CEIL(start, PAGE_SIZE);
		     (un)f + PAGE_SIZE <= start + size;
		     f = (void *)((un)f + PAGE_SIZE)) {
			if (pfree(f, PAGE_SIZE)) {
				tracepanicmsg("freeallmem");
				panic();
			}

			/* Log how many MiB have been loaded. Logging more
			   frequently, for example using KiB, makes the overall
			   freeing a lot slower because it has to go through
			   the serial console a lot of times. */
			if (!((done += PAGE_SIZE) % (1024 * 1024))) {
				(void)consolewrite("\r");
				(void)consolewrite(MEM_LOAD_LOG_PRE);
				(void)consolewriteintb10u(done
				                          / (1024 * 1024));
				(void)consolewrite("M");
			}
		}

		/* Once done, print MiB with 2 decimal places to give more
		   preciseness and print start and end addresses. */
		(void)consolewrite("\r");
		(void)consolewrite(MEM_LOAD_LOG_PRE);
		(void)consolewriteintb10u(done / (1024 * 1024));
		(void)consolewrite(".");
		(void)consolewriteintb10u((done % (1024 * 1024))
		                    * 100 / (1024 * 1024));
		(void)consolewrite("M (");
		(void)consolewriteintb16(start);
		(void)consolewrite("-");
		(void)consolewriteintb16(start + size);
		(void)consolewrite(")  OK\n");
	}

	return 0;
}

void
physicalmem(void)
{
	memswitch(NULL, 0);
}
