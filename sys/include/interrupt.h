#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <arch/types.h>

#define INTERRUPT_TYPE_UNKNOWN   0x00
#define INTERRUPT_TYPE_EXCEPTION 0x01
#define INTERRUPT_TYPE_HARDWARE  0x02
#define INTERRUPT_TYPE_SOFTWARE  0x03
#define INTERRUPT_TYPE_TIMER     0x04
#define INTERRUPT_TYPE_SYSCALL   0x05

/* Disables interrupts. */
void disableinterrupts(void);

/* Enables interrupts. */
void enableinterrupts(void);

/* Returns the interrupt type code. */
u8 getinterrupttype(void);

/* Returns 1 if interrupts are enabled and 0 otherwise. */
int interruptsenabled(void);

#endif
