#ifndef _PROCESS_H
#define _PROCESS_H

#include <arch/ctx.h>
#include <arch/page.h>
#include <arch/types.h>
#include <spinlock.h>

/* Process virtual address space:
     + 0x0
     |
     | (kr-x) Trampoline.
     |
     + PAGE_SIZE
     |
     | (krw-) Trap frame.
     |
     + 2 * PAGE_SIZE
     |
     | (urwx) Program.
     |
     + 2 * PAGE_SIZE + [Program size]
     |
     | (urw-) Free memory.
     |
     + PAGE_VADDR_MAX
*/
#define PROC_VAS_TRAMPOLINE      (0 * PAGE_SIZE)
#define PROC_VAS_TRAP_FRAME      (1 * PAGE_SIZE)
#define PROC_VAS_FIRST_FREE_PAGE (2 * PAGE_SIZE)

#define PID_MAX (u16)(~0)

enum processstate {
	READY,
	RUNNING,
	BLOCKED,
	TERMINATED,
};

struct process {
	/* The process ID. */
	u16 pid;

	/* The spinlock of the process, used by the scheduler. */
	struct lock lock;

	/* The process state. */
	enum processstate state;

	/* The parent process.. */
	struct process *parent;

	/* The pointer to the root table of the virtual page tree. */
	pageentry *pagetree;

	/* The context of the core before switching to the process. */
	u8 kctx[CTX_SIZE];

	/* The context of the process in user mode. */
	u8 uctx[CTX_SIZE];

	/* The pointer to the next process. This is needed to create linked
	   list of processes. */
	struct process *n;
};

/* Allocates a child process of process parent, or a standalone process with no
   parent if it is NULL, of state READY and sets p to it. Returns -1 in case of
   error or 0 otherwise. */
s8 allocprocess(struct process **p, struct process *parent);

/* Returns the linked list containing all the processes. */
struct process *processes(void);

/* Returns the physical address of the trap frame of process p or NULL if it
   does not exist. */
void *trapframe(struct process *p);

#endif
