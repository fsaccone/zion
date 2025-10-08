#ifndef _MEMSWITCH_H
#define _MEMSWITCH_H

#include <arch/page.h>

/* Switches to virtual memory using page tree ptree. The page tree needs to be
   initialized in a way this function is mapped at the same address as its
   physical one. */
void memswitch(pageentry *ptree);

#endif
