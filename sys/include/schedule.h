#ifndef _SCHEDULE_H
#define _SCHEDULE_H

/* The process scheduler. It never returns, it loops through every process
   instead, acting as needed based on each process state. */
void schedule(void);

/* Switches to the context of the running core that was saved when it switched
   to the process, effectively continuing the execution of the scheduler. */
void nextschedule(void);

#endif
