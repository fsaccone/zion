#ifndef _TIMER_H
#define _TIMER_H

/* It configures the next timer interrupt. */
void setupnexttimer(void);

/* It handles a timer interrupt. */
void timer(void);

#endif
