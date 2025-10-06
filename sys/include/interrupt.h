#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <arch/types.h>

#define INTERRUPT_ARGS 5

#define INTERRUPT_TYPE_EXCEPTION 0x00
#define INTERRUPT_TYPE_SYSCALL   0x01
#define INTERRUPT_TYPE_HARDWARE  0x02
#define INTERRUPT_TYPE_TIMER     0x03

/* Disables interrupts. */
void disableinterrupts(void);

/* Enables interrupts. */
void enableinterrupts(void);

/* Returns the pointer to the handler function which was set by
   setinterrupthandler. */
void *getinterrupthandler(void);

/* The interrupt handler. This will do architecture-specific actions to
   eventually call the function set by setinterrupthandler. */
void handleinterrupt(void);

/* Returns an array of size INTERRUPT_ARGS containing the values of the first
   INTERRUPT_ARGS architecture-specific argument registers of the caller
   context. This function has to be called first inside the interrupt handler,
   because having code before it may alter the values of the registers. */
ureg *interruptargs(void);

/* Returns 1 if the interrupt comes from user mode or 0 if it comes from kernel
   mode. */
u8 interruptisuser(void);

/* Returns 1 if interrupts are enabled and 0 otherwise. */
u8 interruptsenabled(void);

/* Returns the interrupt type code. */
u8 interrupttype(void);

/* Sets handler as the function handleinterrupts calls to handle interrupts. */
void setinterrupthandler(void (*handler)(void));

/* Sets the first architecture-specific argument register of the caller context
   to r. If it is not called, the register retains its original value. */
void setinterruptreturn(ureg r);

/* Does not return. Makes the running core in a state where it just waits for
   an interrupt to reactivate it. */
void waitforinterrupt(void);

#endif
