#ifndef _PAGETABLE_H
#define _PAGETABLE_H

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

/* Allocates a frame and adds a page entry pointing to it with options opts to
   page table pt. It returns the pointer to the page entry or NULL in case of
   error. */
pageentry *allocpage(pageentry *pt[PAGE_TABLE_ENTRIES],
                     struct pageoptions opts);

/* Makes page table entry pte invalid and frees the frame it points to and all
   the parent tables in page tree pt which became empty after the removal of
   pte. */
void freepage(pageentry *pte, pageentry *pt[PAGE_TABLE_ENTRIES]);

/* Makes pt an empty page table. */
void pagetable(pageentry *pt[PAGE_TABLE_ENTRIES]);

/* Returns the first page entry in page tree ptree such that the check
   function called with the extra parameter does not return 0. Returns NULL if
   no such entry is found. */
pageentry *walkpagetree(pageentry *ptree[PAGE_TABLE_ENTRIES],
                        u8 (*check)(pageentry, void *),
                        void *extra);

#endif
