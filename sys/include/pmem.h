#ifndef _PMEM_H
#define _PMEM_H

#include <arch.h>

#define NULL ((void *)0)

/* Allocate as much adjacent frames as needed to fit size s and return the
   pointer to the first one */
void *palloc(uintn s);

/* Free the p frame */
void pfree(void *p);

/* Free all frames in range s-e */
void pfreerange(void *s, void *e);

/* Sets all n bytes starting from s to c and returns s */
void *pmemset(void *s, int c, uintn n);

#endif
