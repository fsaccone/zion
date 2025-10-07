#ifndef __KVMEM_H
#define __KVMEM_H

#include <arch/types.h>

/* Allocates a page tree, initializes it with a kernel virtual address space
   and switches to it. Returns -1 on error or 0 otherwise. */
s8 kvmem(void);

#endif
