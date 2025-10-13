#ifndef _PMEM_H
#define _PMEM_H

#include <arch/page.h>
#include <arch/types.h>

#define MAX_PALLOC_FRAMES 4
#define MAX_PALLOC        (MAX_PALLOC_FRAMES * PAGE_SIZE)

#define NULL ((void *)0)

/* It allocates as much adjacent frames as needed to fit size s starting with
   a frame aligned to align or to PAGE_SIZE if align is 0. Allocated frames are
   filled with zeros. On success, a pointer to the first frame is returned. On
   failure, NULL is returned and the panic message is set. */
void *palloc(uptr s, uptr align);

/* It allocates all free memory pages, thus filling all used memory with
   zeros. */
void pcleanup(void);

/* It frees enough frames starting with f to fit size s. On success, 0 is
   returned. On failure, -1 is returned and the panic message is set. */
s8 pfree(void *f, uptr s);

/* If the memory block at a is equal to the memory block at b, 0 is returned.
   If the memory block at a is less than the memory block at b, a negative
   number is returned. If the memory block at a is greater than the memory
   block at b, a positive number is returned. Memory blocks are made of n
   bytes. */
s8 pmemcmp(void *a, void *b, uptr n);

/* It copies n bytes from s to d. */
void pmemcpy(void *d, void *s, uptr n);

/* It sets the first n bytes of d to c. */
void pmemset(void *d, u8 c, uptr n);

#endif
