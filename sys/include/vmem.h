#ifndef _VMEM_H
#define _VMEM_H

#include <arch/page.h>
#include <arch/types.h>
#include <math.h>

#define STACK_SIZE CEIL(8192, PAGE_SIZE)

#define VADDR_TRAMPOLINE      (0 * PAGE_SIZE)
#define VADDR_TRAP_FRAME      (1 * PAGE_SIZE)
#define VADDR_FIRST_FREE_PAGE (2 * PAGE_SIZE)
#define VADDR_STACK_END       ((uptr)(~0))
#define VADDR_STACK_START     (VADDR_STACK_END - STACK_SIZE)

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

/* Initializes the default virtual address space for page tree ptree:
     1. Map VADDR_TRAMPOLINE to trampoline frame trampoline.
     2. Map VADDR_TRAP_FRAME to trap frame tframe.
     3. Allocate stack of size STACK_SIZE through separate frames.
     4. Map addresses VADDR_STACK_START to VADDR_STACK_END to their respective
        frames.
   Returns -1 in case of error or 0 otherwise. */
s8 initvaddrspace(pageentry *ptree, void *trampoline, void *tframe);

/* Returns the physical address pointed by virtual address vaddr in page tree
   ptree, or NULL in case of error. */
void *paddr(pageentry *ptree, uptr vaddr);

/* Validates page pointed by virtual address vaddr of page tree ptree using
   options opts, mapping it to the paddr physical address. Allocates missing
   intermediate page tables. Returns -1 in case of error and 0 otherwise. */
s8 vmap(pageentry *ptree, uptr vaddr, void *paddr, struct pageoptions opts);

/* Invalidates allocated page pointed by virtual address vaddr of page tree
   ptree, freeing the page tables which were emptied after the operation.
   Returns -1 in case of error and 0 otherwise. */
s8 vunmap(pageentry *ptree, uptr vaddr);

#endif
