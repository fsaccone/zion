#ifndef _PAGEFAULT_H
#define _PAGEFAULT_H

#include <arch/types.h>

/* The page fault handler. The u argument is 1 when the page fault is caused by
   code in user mode. */
void pagefault(u8 u);

#endif
