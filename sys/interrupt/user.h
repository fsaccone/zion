#ifndef __USER_H
#define __USER_H

#include <interrupt.h>

/* The interrupt handler for interrupts caused by user mode code with arguments
   args. */
void userinterrupt(un args[INTERRUPT_ARGS]);

#endif
