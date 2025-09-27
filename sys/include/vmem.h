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

/* Makes ptable an empty page table. */
void pagetable(pageentry *ptable[PAGE_TABLE_ENTRIES]);

/* Returns the first page entry in page tree ptree in levels [minlvl, maxlvl]
   such that the check function called with the extra parameter does not return
   0. Returns NULL if no such entry is found. */
pageentry *walkpagetree(pageentry *ptree[PAGE_TABLE_ENTRIES],
                        u8 minlvl, u8 maxlvl,
                        s8 (*check)(pageentry, void *),
                        void *extra);

#endif
