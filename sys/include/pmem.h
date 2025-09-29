#ifndef _PMEM_H
#define _PMEM_H

#include <arch/page.h>
#include <arch/types.h>

#define MAX_PALLOC_FRAMES 4
#define MAX_PALLOC        (MAX_PALLOC_FRAMES * PAGE_SIZE)

#define NULL ((void *)0)

/* Allocate as much adjacent frames as needed to fit size s, fill with zeros
   and return the pointer to the first one, which will be aligned to align or
   to PAGE_SIZE if align is 0. Returns NULL in case of error. */
void *palloc(uptr s, uptr align);

/* Allocates all free memory pages, thus filling all used memory with zeros. */
void pcleanup(void);

/* Free enough frames from p to fit size s. Returns -1 in case of error and 0
   otherwise. */
s8 pfree(void *f, uptr s);

/* Copy n bytes from s to d. */
void pmemcpy(void *d, void *s, uptr n);

/* Sets all n bytes starting from s to c. */
void pmemset(void *s, u8 c, uptr n);

#endif
