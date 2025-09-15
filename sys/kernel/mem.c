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

u8
freeallmem(void)
{
	u8 i;
	uptr freemem[FREE_MEMORY_REGIONS_LEN][2] = { FREE_MEMORY_REGIONS };

	for (i = 0; i < FREE_MEMORY_REGIONS_LEN; i++) {
		uptr start = freemem[i][0],
		     end   = freemem[i][1],
		     done  = 0;
		void *f;
		u8 decpart;

		for (f = (void *)CEIL(start, PAGE_SIZE);
		     (uptr)f + PAGE_SIZE <= end;
		     f = (void *)((uptr)f + PAGE_SIZE)) {
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
		(void)consolewrite("\r");
		(void)consolewrite(MEM_LOG_PRE);
		(void)consolewriteintb10u(done / (1024 * 1024));
		(void)consolewrite(".");
		decpart = ((done % (1024 * 1024)) * 100) / (1024 * 1024);
		if (decpart < 10)
			(void)consolewrite("0");
		(void)consolewriteintb10u(decpart);
		(void)consolewrite("M  [");
		(void)consolewriteintb16(start);
		(void)consolewrite(" - ");
		(void)consolewriteintb16(end);
		(void)consolewrite("]\n");
	}

	return 0;
}

void
physicalmem(void)
{
	memswitch(NULL, 0);
}
