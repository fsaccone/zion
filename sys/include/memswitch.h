#ifndef _MEMSWITCH_H
#define _MEMSWITCH_H

#include <arch/page.h>

/* Switches to virtual memory using page tree ptree. The page tree needs to be
   initialized to:
     - Map this function at the same address as its physical one.
     - Map a stack at the maximum address. */
void memswitch(pageentry *ptree);

#endif
