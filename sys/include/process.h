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
	u32 pid;
	enum processstate state;
	void *pagetable;
};

/* Initializes first process and returns its pointer. Panics if run more than
   one time */
struct process *initprocess(void);

#endif
