#ifndef _TIMER_H
#define _TIMER_H

/* Does some architecture-specific setup for the next timer interrupt. It is
   best to call this first in the timer interrupt handler. */
void setupnexttimer(void);

#endif
