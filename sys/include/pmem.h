#ifndef _PMEM_H
#define _PMEM_H

#include <arch.h>

#define MAX_PALLOC_FRAMES 4
#define MAX_PALLOC        (MAX_PALLOC_FRAMES * PAGE_SIZE)

#define NULL ((void *)0)

/* Allocate as much adjacent frames as needed to fit size s, fill with zeros
   and return the pointer to the first one */
void *palloc(uintn s);

/* Free enough frames from p to fit size s */
void pfree(void *p, uintn s);

/* Free all frames in range s-e */
void pfreerange(void *s, void *e);

/* Sets all n bytes starting from s to c */
void pmemset(void *s, int c, uintn n);

#endif
