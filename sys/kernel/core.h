#ifndef __CORE_H
#define __CORE_H

#include <arch/types.h>

/* The main function of every CPU core with core ID c to be called after system
   has been initialized. */
void coremain(u16 c);

#endif
