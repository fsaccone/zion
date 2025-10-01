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

/* Page entry in the context of its page tree. */
struct pte {
	/* Its page table. */
	pageentry *ptable;

	/* The level (starting at 0) of its page table. */
	u8 l;

	/* The index of the entry in its page table. */
	u32 i;
};

/* Allocates a page table full of invalid entries and returns its address.
   Returns NULL in case of error. */
pageentry *allocpagetable(void);

/* Validates page pointed by virtual address vaddr of page tree ptree using
   options opts, allocating a frame and making the page entry point to its
   physical address. Allocates missing intermediate page tables. Returns -1 in
   case of error and 0 otherwise. */
s8 valloc(pageentry ptree[PAGE_TABLE_ENTRIES], uptr vaddr,
          struct pageoptions opts);

/* Invalidates allocated page pointed by virtual address vaddr of page tree
   ptree, freeing the physical frame pointed by it and all the page tables
   which were emptied after the operation. Returns -1 in case of error and 0
   otherwise. */
s8 vfree(pageentry ptree[PAGE_TABLE_ENTRIES], uptr vaddr);

/* Sets o to the first page entry in page tree ptree up to maxlvl level (the
   root level is 0) such that the check function called with the entry and the
   extra parameter does not return 0. Returns 0 if such entry is found, 1 if it
   is not found or -1 in case of error. The check function may raise an error
   by returning -1. */
s8 walkpagetree(struct pte *o, pageentry ptree[PAGE_TABLE_ENTRIES], u8 maxlvl,
                s8 (*check)(struct pte, void *), void *extra);

#endif
