/* ARCHITECTURE-SPECIFIC */
#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#define INTERRUPT_TYPE_UNKNOWN   0x00
#define INTERRUPT_TYPE_EXCEPTION 0x01
#define INTERRUPT_TYPE_HARDWARE  0x02
#define INTERRUPT_TYPE_SOFTWARE  0x03
#define INTERRUPT_TYPE_TIMER     0x04

/* Disables device interrupts */
void disableinterrupts(void);

/* Enables device interrupts */
void enableinterrupts(void);

/* Returns 1 if interrupts are enabled and 0 otherwise */
int interruptsenabled(void);

#endif
