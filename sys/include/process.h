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

/* Node of a linked list of pages. */
struct pagenode {
	/* The virtual address of the page. */
	uptr p;

	/* The physical address of the allocated frame pointed by the p virtual
	   address, or NULL if the frame pointed by v is not allocated. */
	void *f;

	/* The next node. */
	struct pagenode *n;
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

	/* The process state. */
	enum processstate state;

	/* The parent process.. */
	struct process *parent;

	/* The pointer to the root table of the virtual page tree. */
	pageentry *pagetree;

	/* The linked list containing the mapped virtual address space
	   pages. */
	struct pagenode *vas;

	/* The context of the core before switching to the process. */
	u8 kctx[CTX_SIZE];

	/* The context of the process in user mode. */
	u8 uctx[CTX_SIZE];
};

/* Allocates a child process of process parent, or a standalone process with no
   parent if it is NULL, of state READY and sets p to it. Returns -1 in case of
   error or 0 otherwise. */
s8 createprocess(struct process **p, struct process *parent);

/* Returns the linked list containing all the processes. */
struct processnode *processes(void);

/* Returns the physical address of the trap frame of process p or NULL if it
   does not exist. */
void *trapframe(struct process *p);

#endif
