#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <arch/types.h>

#define INTERRUPT_TYPE_EXCEPTION  0x00
#define INTERRUPT_TYPE_HARDWARE   0x02
#define INTERRUPT_TYPE_TIMER      0x03
#define INTERRUPT_TYPE_PAGE_FAULT 0x04

/* Disables interrupts. */
void disableinterrupts(void);

/* Enables interrupts. */
void enableinterrupts(void);

/* The interrupt handler. */
void interrupt(void);

/* Returns the physical address of interrupt. */
void *interruptbase(void);

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
