#include <vmem.h>

#include <arch.h>
#include <pmem.h>

#define PAGE_TABLE_ENTRIES 512

void *
createpagetable(void)
{
	/* Since palloc allocates frames aligned with PAGE_SIZE=4096, it is
	   assured that the page table is also aligned to 4096. Also, palloc
	   fills all allocated frames with zeros, and that is what we want. */
	return palloc(PAGE_TABLE_ENTRIES * sizeof(uintn));
}
