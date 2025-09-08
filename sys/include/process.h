#ifndef _PROCESS_H
#define _PROCESS_H

#include <arch/types.h>

/* Maximum 16 bits unsigned integer. */
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
	/* The process ID. */
	u16 pid;

	/* The process state. */
	enum processstate state;

	/* The pointer to the root table of the virtual page tree. */
	void *pagetable;

	/* The CPU context. */
	void *ctx;

	/* A linked list containing the children processes of the process. */
	struct processlist *children;
};

/* Creates a child process of process parent and returns its pointer. Returns
   NULL in case of error. */
struct process *createprocess(struct process *parent);

/* Initializes first process and returns its pointer. Returns NULL in case of
   error. */
struct process *initprocess(void);

#endif
