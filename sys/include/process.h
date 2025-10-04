#ifndef _PROCESS_H
#define _PROCESS_H

#include <arch/ctx.h>
#include <arch/page.h>
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
	pageentry *pagetree;

	/* The CPU context. */
	u8 ctx[CTX_SIZE];

	/* A linked list containing the children processes of the process. */
	struct processlist *children;
};

/* Allocates a child process of process parent with program base address pbase
   and size psize. If parent is NULL, it creates the init process, parent of
   every other process: if it was already created, an error is raised. Returns
   -1 in case of error or 0 otherwise. */
s8 allocprocess(void *pbase, uptr psize, struct process *parent);

#endif
