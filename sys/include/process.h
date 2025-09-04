#ifndef _PROCESS_H
#define _PROCESS_H

#include <types.h>

/* Maximum 16 bits unsigned integer */
#define PID_MAX 65535

enum processstate {
	CREATED,
	READY,
	RUNNING,
	BLOCKED,
	TERMINATED,
};

struct processlist {
	struct process *p;
	struct processlist *n;
};

struct process {
	/* The process ID */
	u16 pid;

	/* The process state */
	enum processstate state;

	/* The pointer to the root table of the virtual page tree */
	void *pagetable;

	/* A linked list containing the children processes of the process */
	struct processlist *children;
};

/* Initializes first process and returns its pointer. Panics if run more than
   one time */
struct process *initprocess(void);

#endif
