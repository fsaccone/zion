#ifndef _VMEM_H
#define _VMEM_H

#include <arch/types.h>

/* Switches to virtual memory using page tree pt or to physical memory if pt is
   NULL. The id ID may be used to help the TLB keep things straight, and may
   match the PID of the process owning the page tree. */
void memswitch(void *pt, u8 id);

#endif
