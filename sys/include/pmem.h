#ifndef _PMEM_H
#define _PMEM_H

/* Allocate one frame and return its address */
void *palloc(void);

/* Free the p frame */
void pfree(void *p);

/* Free all frames in range s-e */
void pfreerange(void *s, void *e);

#endif
