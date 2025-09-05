#ifndef _PAGETABLE_H
#define _PAGETABLE_H

#include <arch/types.h>

struct pageoptions {
	/* If 1, the page is for user-mode; otherwise, it is for kernel-mode */
	u8 u : 1;

	/* If 1, the page is readable */
	u8 r : 1;

	/* If 1, the page is writable */
	u8 w : 1;

	/* If 1, the page is executable */
	u8 x: 1;

	u8 reserved : 4;
};

/* Allocates a frame and adds a page entry pointing to it with options opts to
   page table pt. It returns the pointer to the page entry or NULL if page
   table is full */
void *allocpage(void *pt, struct pageoptions opts);

/* Creates an empty page table full of invalid entries and returns its
   pointer */
void *createpagetable(void);

/* Makes page table entry pte invalid and frees the frame it points to and all
   the parent tables in page tree pt which became empty after the removal of
   pte */
void freepage(void *pte, void *pt);

#endif
