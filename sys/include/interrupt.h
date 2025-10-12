#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <arch/types.h>

/* Disables interrupts. */
void disableinterrupts(void);

/* Enables interrupts. */
void enableinterrupts(void);

/* Returns 1 if interrupts are enabled and 0 otherwise. */
u8 interruptsenabled(void);

/* The entry point of the kernel interrupt handler. */
void kernelinterrupt(void);

/* The entry point of the user interrupt handler. */
void userinterrupt(void);

#endif
