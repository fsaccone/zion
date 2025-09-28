#ifndef _VMEM_H
#define _VMEM_H

#include <arch/page.h>
#include <arch/types.h>

struct pageoptions {
	/* If 1, the page is for user-mode; otherwise, it is for
	   kernel-mode. */
	u8 u : 1;

	/* If 1, the page is readable. */
	u8 r : 1;

	/* If 1, the page is writable. */
	u8 w : 1;

	/* If 1, the page is executable. */
	u8 x: 1;

	u8 reserved : 4;
};

/* The data of an entry in the context of its page table. */
struct pte {
	/* The pointer to the entry. */
	pageentry *e;

	/* The level (starting at 1) of its page table. */
	u8 l;

	/* The index of the entry in its page table. */
	u32 i;
};

/* Makes ptable an empty page table. */
void pagetable(pageentry *ptable[PAGE_TABLE_ENTRIES]);

/* Returns the first page entry in page tree ptree in levels [minlvl, maxlvl]
   such that the check function called with the extra parameter does not return
   0. Returns NULL if no such entry is found. */
pageentry *walkpagetree(pageentry *ptree[PAGE_TABLE_ENTRIES],
                        u8 minlvl, u8 maxlvl,
                        s8 (*check)(struct pte, void *),
                        void *extra);

#endif
