#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <arch/types.h>
#include <driver.h>

#define INTERRUPT_ARGS 5

#define INTERRUPT_TYPE_EXCEPTION 0x00
#define INTERRUPT_TYPE_SYSCALL   0x01
#define INTERRUPT_TYPE_HARDWARE  0x02
#define INTERRUPT_TYPE_SOFTWARE  0x03
#define INTERRUPT_TYPE_TIMER     0x04

/* Disables interrupts. */
void disableinterrupts(void);

/* Enables interrupts. */
void enableinterrupts(void);

/* Returns an array of size INTERRUPT_ARGS containing the values of the first
   INTERRUPT_ARGS architecture-specific argument registers of the caller
   context. This function has to be called first inside the interrupt handler,
   because having code before it may alter the values of the registers. */
un *interruptargs(void);

/* Sets drv to the driver of the device which caused the interrupt and returns
   0 normally and returns -1 if the interrupt type is not HARDWARE or the
   device is unknown. */
u8 interruptdriver(struct driver *drv);

/* Returns 1 if the interrupt comes from user mode or 0 if it comes from kernel
   mode. */
u8 interruptisuser(void);

/* Returns 1 if interrupts are enabled and 0 otherwise. */
u8 interruptsenabled(void);

/* Returns the interrupt type code. */
u8 interrupttype(void);

/* Sets the first architecture-specific argument register of the caller context
   to r. If it is not called, the register retains its original value. */
void setinterruptreturn(un r);

#endif
