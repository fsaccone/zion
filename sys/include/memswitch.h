#ifndef _MEMSWITCH_H
#define _MEMSWITCH_H

#include <arch/page.h>

/* It switches to virtual memory using page tree ptree. The ptree page tree
   needs to map this function to the same address as the calling one. */
void memswitch(pageentry *ptree);

/* It switches to physical memory. The active page tree needs to map this
   function to the same address as its physical one. */
void pmemswitch(void);

#endif
