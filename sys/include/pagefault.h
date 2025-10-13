#ifndef _PAGEFAULT_H
#define _PAGEFAULT_H

#include <arch/types.h>

/* It handles a kernel page fault if u is 0 or a user page fault otherwise. */
void pagefault(u8 u);

#endif
