#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include <arch/page.h>
#include <process.h>

/* If the calling core owns a process, a pointer to the process is returned.
   The lock of the process must be acquired by the calling core. If the calling
   core does not own a process, NULL is returned. */
struct process *coreprocess(void);

/* It loops infinitely through every allocated process, making the calling core
   switch to the first READY process it finds. The core may eventually switch
   back here by calling nextschedule. */
void schedule(void);

/* It makes the calling core switch back to the scheduler. If the calling core
   did not switch to a process before, it does nothing. */
void nextschedule(void);

#endif
