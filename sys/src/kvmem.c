#include "kvmem.h"

#include <arch/page.h>
#include <arch/types.h>
#include <machine/mem.h>
#include <memswitch.h>
#include <panic.h>
#include <pmem.h>
#include <vmem.h>

/* Map memory regions of memory-mapped IO in page tree ptree to their
   respective addresses. Returns -1 in case of error or 0 otherwise. */
static s8 mapio(pageentry *ptree);

s8
mapio(pageentry *ptree)
{
	struct pageoptions opts = {
		.u = 0,
		.r = 1,
		.w = 1,
		.x = 0,
	};

	/* Map PLIC. */
	if (vmap(ptree, PLIC, (void *)PLIC, opts))
		goto panic;

	/* Map UART. */
	if (vmap(ptree, UART0, (void *)UART0, opts))
		goto panic;

	return 0;

panic:
	tracepanicmsg("mapio");
	return -1;
}

s8
kvmem(void)
{
	pageentry *ptree;
	void *tframe;
	uptr a;
	struct pageoptions opts;
	uptr i, freemem[FREE_MEMORY_REGIONS_LEN][2] = FREE_MEMORY_REGIONS;

	if (!(ptree = allocpagetable()))
		goto panic;

	if (!(tframe = palloc(PAGE_SIZE, 0)))
		goto panic;

	/* Do default initialization. */
	if (allocvas(ptree, tframe, 1))
		goto panic;

	/* Map kernel and raminit texts. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 0;
	opts.x = 1;
	for (a = KERNEL_START;
	     a < KERNEL_START + KERNEL_SIZE;
	     a += PAGE_SIZE) {
		if (vmap(ptree, a, (void *)a, opts))
			goto panic;
	}
	for (a = RAMINIT_START;
	     a < RAMINIT_START + RAMINIT_SIZE;
	     a += PAGE_SIZE) {
		if (vmap(ptree, a, (void *)a, opts))
			goto panic;
	}

	/* Map free memory regions. */
	opts.u = 0;
	opts.r = 1;
	opts.w = 1;
	opts.x = 0;
	for (i = 0; i < FREE_MEMORY_REGIONS_LEN; i++) {
		uptr start = freemem[i][0],
		     end   = freemem[i][0];

		for (a = start; a < end; a += PAGE_SIZE) {
			if (vmap(ptree, a, (void *)a, opts))
				goto panic;
		}
	}

	if (mapio(ptree))
		goto panic;

	memswitch(ptree);

	return 0;

panic:
	tracepanicmsg("kvmem");
	return -1;
}
