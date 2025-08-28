#ifndef _PMEM_H
#define _PMEM_H

/* Allocate PAGE_SIZE bytes of physical memory */
void *palloc(void);

/* Free the p physical page */
void pfree(void *p);

#endif
