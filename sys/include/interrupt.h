#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <arch/types.h>

/* It disables interrupts for the calling core. */
void disableinterrupts(void);

/* It enables interrupts for the calling core. */
void enableinterrupts(void);

/* It returns 1 if interrupts are enabled for the calling core or 0
   otherwise. */
u8 interruptsenabled(void);

/* The entry point to kernel interrupts. */
void kernelinterrupt(void);

/* The entry point to user interrupts, called from trampoline. */
void userinterrupt(void);

#endif
