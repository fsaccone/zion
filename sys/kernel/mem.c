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

/* (10M) */
#define LOG_SIZE (10 * 1024 * 1024)

int
freeallmem(void)
{
	int i, maxloglen = 0;
	un freemem[FREE_MEMORY_REGIONS_LEN][2] = { FREE_MEMORY_REGIONS };

	for (i = 0; i < FREE_MEMORY_REGIONS_LEN; i++) {
		un start = freemem[i][0],
		   end   = freemem[i][1],
		   done  = 0;
		void *f;
		int loglen = 0;

		for (f = (void *)CEIL(start, PAGE_SIZE);
		     (un)f + PAGE_SIZE <= end;
		     f = (void *)((un)f + PAGE_SIZE)) {
			if (pfree(f, PAGE_SIZE)) {
				tracepanicmsg("freeallmem");
				panic();
			}

			/* Log how many MiB have been loaded every LOG_SIZE
			   MiB. Logging more frequently makes the overall
			   freeing a lot slower because it has to go through
			   the serial console a lot of times. */
			if (!((done += PAGE_SIZE) % LOG_SIZE)) {
				(void)consolewrite("\r");
				(void)consolewrite(MEM_LOG_PRE);
				(void)consolewriteintb10u(done
				                          / (1024 * 1024));
				(void)consolewrite("M");
			}
		}

		/* Once done, print MiB with 2 decimal places to give more
		   preciseness and print start and end addresses. */
		loglen += consolewrite("\r");
		loglen += consolewrite(MEM_LOG_PRE);
		loglen += consolewriteintb10u(done / (1024 * 1024));
		loglen += consolewrite(".");
		loglen += consolewriteintb10u((done % (1024 * 1024))
		                    * 100 / (1024 * 1024));
		loglen += consolewrite("M");

		if (loglen > maxloglen) {
			maxloglen = loglen;
		} else {
			int j;

			for (j = 0; j < maxloglen - loglen; j++)
				consolewrite(" ");
		}

		consolewrite("  [");
		consolewriteintb16(start);
		consolewrite(" - ");
		consolewriteintb16(end);
		consolewrite("]\n");
	}

	return 0;
}

void
physicalmem(void)
{
	memswitch(NULL, 0);
}
