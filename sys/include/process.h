#ifndef _PROCESS_H
#define _PROCESS_H

#include <arch/ctx.h>
#include <arch/page.h>
#include <arch/types.h>
#include <spinlock.h>
#include <vmem.h>

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
     | (????) Mapped memory.
     |
     + ceil
     |
     | (////) Unmapped memory.
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

	/* The virtual address of the first unmapped page of the virtual
	   address space. */
	uptr ceil;

	/* The context which is saved when switching back to the scheduler
	   after a user interrupt, and loaded the other way around. */
	u8 ctx[CTX_SIZE];

	/* The pointers to the previous and next processes. This is needed to
	   create linked lists of processes. */
	struct process *p;
	struct process *n;
};

/* Allocates a child process of process parent of state READY and sets p to it.
   If parent is null, the process is referred to as the "init" process: it has
   no parents and automatically adopts all orphaned processes. The init process
   must only be allocated once. Returns -1 in case of error or 0 otherwise. */
s8 allocprocess(struct process **p, struct process *parent);

/* Frees process p and all its allocated memory. Returns -1 in case of error or
   0 otherwise. */
s8 freeprocess(struct process *p);

/* Increases ceil of process p by a page, mapping it to the paddr physical
   address and setting its options to opts. It returns -1 in case of error or
   0 otherwise, setting it to the virtual address of the newly mapped page if
   it is not NULL. */
s8 growprocess(uptr *o, struct process *p, void *f, struct pageoptions opts);

/* Returns the linked list containing all the processes. */
struct process *processes(void);

/* Returns the physical address of the trap frame of process p or NULL if it
   does not exist. */
void *trapframe(struct process *p);

#endif
