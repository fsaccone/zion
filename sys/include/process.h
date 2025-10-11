#ifndef _PROCESS_H
#define _PROCESS_H

#include <arch/ctx.h>
#include <arch/page.h>
#include <arch/types.h>
#include <pmem.h>
#include <spinlock.h>

/* Maximum 16 bits unsigned integer. */
#define PID_MAX 65535

enum processstate {
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

	/* The spinlock of the process, used by the scheduler. */
	struct lock lock;

	/* A linked list containing the frames which were allocated for the
	   process. */
	struct framenode *allocated;

	/* The process state. */
	enum processstate state;

	/* The pointer to the root table of the virtual page tree. */
	pageentry *pagetree;

	/* The context of the core before switching to the process. */
	u8 kctx[CTX_SIZE];

	/* The context of the process in user mode. */
	u8 uctx[CTX_SIZE];

	/* A linked list containing the children processes of the process. */
	struct processnode *children;
};

/* Allocates a child process of process parent, or a standalone process with no
   parent if it is NULL, of state READY with program text contained in the text
   linked list of frames. Returns -1 in case of error or 0 otherwise. */
s8 createprocess(struct framenode *text, struct process *parent);

/* Returns the linked list containing all the processes. */
struct processnode *processes(void);

/* Returns the physical address of the trap frame of process p or NULL if it
   does not exist. */
void *trapframe(struct process *p);

#endif
