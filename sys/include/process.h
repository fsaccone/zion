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
	pageentry pagetree[PAGE_TABLE_ENTRIES];

	/* The CPU context. */
	u8 ctx[CTX_SIZE];

	/* A linked list containing the children processes of the process. */
	struct processlist *children;
};

/* Allocates a child process of process parent with program base address pbase.
   Returns -1 in case of error or 0 otherwise. */
s8 allocprocess(void *pbase, struct process *parent);

/* Initializes first process with program base address pbase. Returns -1 in
   case of error or 0 otherwise. */
s8 initprocess(void *pbase);

#endif
