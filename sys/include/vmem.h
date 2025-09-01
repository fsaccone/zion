/* ARCHITECTURE-SPECIFIC */
#ifndef _VMEM_H
#define _VMEM_H

#include <arch.h>

struct pageoptions {
	/* If 1, the page is for user-mode; otherwise, it is for kernel-mode */
	uint8 u : 1;

	/* If 1, the page is readable */
	uint8 r : 1;

	/* If 1, the page is writable */
	uint8 w : 1;

	/* If 1, the page is executable */
	uint8 x: 1;

	uint8 reserved : 4;
};

/* Creates an empty page table full of invalid entries and returns its
   pointer */
void *createpagetable(void);

/* Allocates a frame and adds a page entry pointing to it with options opts to
   page table pt. Returns NULL if page table is full */
void *valloc(void *pt, struct pageoptions opts);

#endif
