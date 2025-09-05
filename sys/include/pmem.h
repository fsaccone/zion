#ifndef _PMEM_H
#define _PMEM_H

#include <arch/page.h>
#include <arch/types.h>

#define MAX_PALLOC_FRAMES 4
#define MAX_PALLOC        (MAX_PALLOC_FRAMES * PAGE_SIZE)

#define NULL ((void *)0)

/* Allocate as much adjacent frames as needed to fit size s, fill with zeros
   and return the pointer to the first one. Returns NULL in case of error. */
void *palloc(un s);

/* Free enough frames from p to fit size s. Returns -1 in case of error and 0
   otherwise. */
int pfree(void *f, un s);

/* Free all frames in range s-e. Returns -1 in case of error and 0
   otherwise. */
int pfreerange(void *s, void *e);

/* Sets all n bytes starting from s to c. */
void pmemset(void *s, u8 c, un n);

#endif
