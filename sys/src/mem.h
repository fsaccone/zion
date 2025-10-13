#ifndef __MEM_H
#define __MEM_H

#include <arch/types.h>

/* It frees all the free memory regions of the machine. On success, 0 is
   returned. On failure, -1 is returned and the panic message is set. */
s8 freeallmem(void);

#endif
