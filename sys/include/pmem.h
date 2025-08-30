#ifndef _PMEM_H
#define _PMEM_H

#define PAGE_CEIL(a)  (void *)(((uintn_t)(a)) & ~(PAGE_SIZE - 1))
#define PAGE_FLOOR(a) (void *)((((uintn_t)(a)) + PAGE_SIZE - 1) \
                               & ~(PAGE_SIZE - 1))

/* Allocate one frame and return its address */
void *palloc(void);

/* Free the p frame */
void pfree(void *p);

/* Free all frames in range s-e */
void pfreerange(void *s, void *e);

#endif
