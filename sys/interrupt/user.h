#ifndef __USER_H
#define __USER_H

#include <arch/types.h>
#include <interrupt.h>

/* The interrupt handler for interrupts caused by user mode code with arguments
   args. */
void userinterrupt(ureg args[INTERRUPT_ARGS]);

#endif
