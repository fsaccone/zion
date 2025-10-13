#ifndef _VMEM_H
#define _VMEM_H

#include <arch/page.h>
#include <arch/types.h>
#include <math.h>

/* The page options. */
struct pageoptions {
	/* If 1, the page is designated for user mode; otherwise, it is for
	   kernel mode. */
	u8 u : 1;

	/* If 1, the page is marked as readable. */
	u8 r : 1;

	/* If 1, the page is marked as writable. */
	u8 w : 1;

	/* If 1, the page is marked as executable. */
	u8 x : 1;

	/* If 1, the page points to a physically allocated frame. */
	u8 a : 1;

	u8 reserved : 3;
};

/* It allocates a page table full of invalid entries. On success, a pointer to
   the allocated page table is returned. On failure, NULL is returned and the
   panic message is set. */
pageentry *allocpagetable(void);

/* It frees the ptable page table. It does not free the children tables: to do
   that, it is better to unmap all the mapped pages using vunmap. On success,
   0 is returned. On failure, -1 is returned and the panic message is set. */
s8 freepagetable(pageentry *ptable);

/* If the page of the ptree page tree defined by the vaddr virtual address is
   mapped to a physical frame, it returns a pointer to it, also copying the
   options of the page to opts if it is not NULL. If vaddr is not mapped, NULL
   is returned. */
void *paddr(struct pageoptions *opts, pageentry *ptree, uptr vaddr);

/* It maps the page of the ptree page tree defined by the vaddr virtual address
   to the paddr physical address, using opts as the options of the page. If any
   missing intermediate page table is missing in the page tree, it is
   allocated and mapped accordingly. On success, 0 is returned. On failure, -1
   is returned and the panic message is set. */
s8 vmap(pageentry *ptree, uptr vaddr, void *paddr, struct pageoptions opts);

/* It unmaps the page of page tree ptree defined by the vaddr virtual address,
   freeing the page tables which were emptied after the operation. On success,
   0 is returned. On failure, -1 is returned and the panic message is set. */
s8 vunmap(pageentry *ptree, uptr vaddr);

#endif
