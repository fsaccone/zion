#ifndef _PROCESS_H
#define _PROCESS_H

#include <arch/ctx.h>
#include <arch/page.h>
#include <arch/types.h>
#include <pmem.h>

/* Maximum 16 bits unsigned integer. */
#define PID_MAX 65535

enum processstate {
	CREATED,
	READY,
	RUNNING,
	BLOCKED,
	TERMINATED,
};

struct processnode {
	struct process *p;
	struct processnode *n;
};

struct process {
	/* The process ID. */
	u16 pid;

	/* A linked list containing the frames which were allocated for the
	   process. */
	struct framenode *allocated;

	/* The process state. */
	enum processstate state;

	/* The pointer to the root table of the virtual page tree. */
	pageentry *pagetree;

	/* The CPU context. */
	u8 ctx[CTX_SIZE];

	/* A linked list containing the children processes of the process. */
	struct processnode *children;
};

/* Allocates a child process of process parent, or a standalone process with no
   parent if it is NULL, with program text contained in the text linked list of
   frames. Returns -1 in case of error or 0 otherwise. */
s8 createprocess(struct framenode *text, struct process *parent);

#endif
