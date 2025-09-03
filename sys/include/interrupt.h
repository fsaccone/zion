/* ARCHITECTURE-SPECIFIC */
#ifndef _INTERRUPT_H
#define _INTERRUPT_H

/* Disables device interrupts */
void disableinterrupts(void);

/* Enables device interrupts */
void enableinterrupts(void);

/* Returns 1 if interrupts are enabled and 0 otherwise */
int interruptsenabled(void);

#endif
