#ifndef _PMEM_H
#define _PMEM_H

/* Allocate PAGE_SIZE bytes of physical memory */
void *palloc(void);

/* Free the p physical page */
void pfree(void *p);

/* Free all pages in range s-e */
void pfreerange(void *s, void *e);

#endif
