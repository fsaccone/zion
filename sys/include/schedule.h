#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include <arch/page.h>
#include <process.h>

/* Returns the process owned by the current core or NULL if the current core
   owns no process. */
struct process *coreprocess(void);

/* The process scheduler. It never returns, it loops through every process
   instead, acting as needed based on each process state. */
void schedule(void);

/* Switches to the context of the running core that was saved when it switched
   to the process, effectively continuing the execution of the scheduler. Does
   noting if the running core did not switch context before. */
void nextschedule(void);

#endif
