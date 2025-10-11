#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <arch/types.h>

/* Disables interrupts. */
void disableinterrupts(void);

/* Enables interrupts. */
void enableinterrupts(void);

/* Returns 1 if the interrupt comes from user mode or 0 if it comes from kernel
   mode. */
u8 interruptisuser(void);

/* Returns 1 if interrupts are enabled and 0 otherwise. */
u8 interruptsenabled(void);

/* Returns the interrupt type code. */
u8 interrupttype(void);

/* The entry point of the kernel interrupt handler. It does
   architecture-specific actions and eventually calls interrupt. */
void kernelinterrupt(void);

/* Does not return. Makes the running core in a state where it just waits for
   an interrupt to reactivate it. */
void waitforinterrupt(void);

#endif
