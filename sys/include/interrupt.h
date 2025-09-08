#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <arch/types.h>

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

/* Returns a NULL terminated array of length INTERRUPT_ARGS containing the
   values of the architecture-specific argument registers used to hold the
   arguments for things like system calls. To get a right copy of the registers
   from the caller context, this function has to be called first inside the
   interrupt handler. */
un *interruptargs(void);

/* Returns 1 if interrupts are enabled and 0 otherwise. */
int interruptsenabled(void);

/* Returns the interrupt type code. */
u8 interrupttype(void);

#endif
