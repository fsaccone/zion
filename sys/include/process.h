#ifndef _PROCESS_H
#define _PROCESS_H

#include <types.h>

enum processstate {
	CREATED,
	READY,
	RUNNING,
	BLOCKED,
	TERMINATED,
};

struct process {
	/* The process ID */
	u32 pid;

	/* The process state */
	enum processstate state;

	/* The pointer to the root table of the virtual page tree */
	void *pagetable;
};

/* Initializes first process and returns its pointer. Panics if run more than
   one time */
struct process *initprocess(void);

#endif
