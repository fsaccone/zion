#include <arch/types.h>
#include <log.h>
#include <machine/mem.h>
#include <pmem.h>

/* Frees all memory regions defined by the machine */
static void freeallmem(void);

void
freeallmem(void)
{
	int i;
	un freemem[FREE_MEMORY_REGIONS_LEN][2] = { FREE_MEMORY_REGIONS };

	for (i = 0; i < FREE_MEMORY_REGIONS_LEN; i++) {
		un start = freemem[i][0],
		   size  = freemem[i][1];

		debugheader("Freeing user memory:");
		debug("From:");
		debugintbase16(start);
		debug("To:");
		debugintbase16((start + size));

		pfreerange((void *)start, (void *)(start + size));
	}
}

void
kmain(void)
{
	debugheader("Kernel start.");

	freeallmem();

	panic("Reached end of kernel execution.");
}
