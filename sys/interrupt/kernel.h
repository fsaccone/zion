#ifndef __KERNEL_H
#define __KERNEL_H

/* The interrupt handler for interrupts caused by kernel mode code. Interrupt
   arguments are voluntarily ignored here. */
void kernelinterrupt(void);

#endif
