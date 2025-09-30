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

/* Validates as much adjacent last-level pages entries of page tree ptree,
   allocating their respective frames and all the intermediate page tables,
   as needed to fit size s. Sets o to the virtual address of the first
   allocated page. Initializes each page entry with options opts. Returns -1 in
   case of error, 1 if there is not enough space in page tree or 0
   otherwise. */
s8 valloc(uptr *o, pageentry ptree[PAGE_TABLE_ENTRIES],
          struct pageoptions opts, uptr s);

/* Frees enough pages in page tree ptree to fit size s starting at the page
   pointed by virtual address vaddr and all the page tables which get empty
   (i.e. full of invalid entries) after the operation. Returns -1 in case of
   error or 0 otherwise. */
s8 vfree(pageentry ptree[PAGE_TABLE_ENTRIES], uptr vaddr, uptr s);

/* Sets o to the first page entry in page tree ptree up to maxlvl level (the
   root level is 0) such that the check function called with the entry and the
   extra parameter does not return 0. Returns 0 if such entry is found, 1 if it
   is not found or -1 in case of error. The check function may raise an error
   by returning -1. */
s8 walkpagetree(struct pte *o, pageentry ptree[PAGE_TABLE_ENTRIES], u8 maxlvl,
                s8 (*check)(struct pte, void *), void *extra);

#endif
