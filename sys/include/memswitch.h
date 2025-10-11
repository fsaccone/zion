#ifndef _MEMSWITCH_H
#define _MEMSWITCH_H

#include <arch/page.h>

/* Switches to virtual memory using page tree ptree. The page tree needs to be
   initialized in a way this function is mapped at the same address as its
   physical one. */
void memswitch(pageentry *ptree);

/* Switches to physical memory. The page tree which is used when this function
   is called needs to map this function to the same address as its physical
   one. */
void pmemswitch(void);

#endif
