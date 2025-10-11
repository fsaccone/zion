#include "kvmem.h"

#include <arch/page.h>
#include <arch/types.h>
#include <interrupt.h>
#include <machine/mem.h>
#include <memswitch.h>
#include <panic.h>
#include <pmem.h>
#include <vmem.h>

s8
kvmem(pageentry **ptree)
{
	uptr a;
	struct pageoptions opts = { 0 };
	uptr i, freemem[FREE_MEMORY_REGIONS_LEN][2] = FREE_MEMORY_REGIONS;

	if (!(*ptree = allocpagetable()))
		goto panic;

	/* Do default initialization. */
	if (allocvas(*ptree, 1))
		goto panic;

	/* Map kernel and raminit. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 1;
	opts.x = 1;
	for (a = KERNEL_START;
	     a < KERNEL_END;
	     a += PAGE_SIZE) {
		if (vmap(*ptree, a, (void *)a, opts))
			goto panic;
	}
	for (a = RAMINIT_START;
	     a < RAMINIT_END;
	     a += PAGE_SIZE) {
		if (vmap(*ptree, a, (void *)a, opts))
			goto panic;
	}

	/* Map free memory regions. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 1;
	opts.x = 0;
	for (i = 0; i < FREE_MEMORY_REGIONS_LEN; i++) {
		uptr start = freemem[i][0],
		     end   = freemem[i][1];

		for (a = start; a < end; a += PAGE_SIZE) {
			if (vmap(*ptree, a, (void *)a, opts))
				goto panic;
		}
	}

	/* Map UART. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 1;
	opts.x = 0;
	if (vmap(*ptree, UART0, (void *)UART0, opts))
		goto panic;

	return 0;

panic:
	tracepanicmsg("kvmem");
	return -1;
}
