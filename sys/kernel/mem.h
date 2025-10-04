#ifndef __MEM_H
#define __MEM_H

#include <arch/types.h>

/* Frees all machine memory regions mapped as usable. Returns -1 in case of
   error or 0 otherwise. */
s8 freeallmem(void);

#endif
