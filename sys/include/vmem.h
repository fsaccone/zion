#ifndef _VMEM_H
#define _VMEM_H

#include <arch/page.h>
#include <arch/types.h>
#include <math.h>

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

/* Allocates a page table full of invalid entries and returns its address.
   Returns NULL in case of error. */
pageentry *allocpagetable(void);

/* Frees page table ptable. Does not free the children tables. Returns -1 in
   case of error or 0 otherwise. */
s8 freepagetable(pageentry *ptable);

/* Returns the physical address pointed by virtual address vaddr in page tree
   ptree, or NULL if it is not mapped. */
void *paddr(pageentry *ptree, uptr vaddr);

/* Validates page pointed by virtual address vaddr of page tree ptree using
   options opts, mapping it to the paddr physical address. Allocates missing
   intermediate page tables. Returns -1 in case of error and 0 otherwise. */
s8 vmap(pageentry *ptree, uptr vaddr, void *paddr, struct pageoptions opts);

/* Copies n pages from virtual address s of page tree sptree to virtual address
   d of page tree dptree, using opts as the options of the destination
   pages. Physical memory is also copied so that the frame of each page is
   duplicated. The s and d addresses must be aligned to PAGE_SIZE. Returns -1
   on error or 0 otherwise. */
s8 vmemcpy(uptr d, pageentry *dptree, uptr s, pageentry *sptree, uptr n,
           struct pageoptions opts);

/* Invalidates allocated page pointed by virtual address vaddr of page tree
   ptree, freeing the page tables which were emptied after the operation.
   Returns -1 in case of error and 0 otherwise. */
s8 vunmap(pageentry *ptree, uptr vaddr);

#endif
