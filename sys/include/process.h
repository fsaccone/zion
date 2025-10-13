#ifndef _PROCESS_H
#define _PROCESS_H

#include <arch/ctx.h>
#include <arch/page.h>
#include <arch/types.h>
#include <spinlock.h>
#include <vmem.h>

/* This is a representation of the process virtual address space:
     + 0x0
     |
     | (krw-a) Trap frame.
     |
     + PAGE_SIZE
     |
     | (kr-x-) Trampoline.
     |
     + 2 * PAGE_SIZE
     |
     | (?????) Mapped memory.
     |
     + [Process ceil]
     |
     | (/////) Unmapped memory.
     |
     + PAGE_VADDR_MAX
*/
#define PROC_VAS_TRAP_FRAME      (0 * PAGE_SIZE)
#define PROC_VAS_TRAMPOLINE      (1 * PAGE_SIZE)
#define PROC_VAS_FIRST_FREE_PAGE (2 * PAGE_SIZE)

#define PID_MAX (u16)(~0)

enum processstate {
	READY,
	RUNNING,
	BLOCKED,
	TERMINATED,
};

/* The process. */
struct process {
	/* The process ID. */
	u16 pid;

	/* The spinlock of the process. */
	struct lock lock;

	/* The process state. */
	enum processstate state;

	/* The parent process. If NULL, the process is referred to as the
	   "init" process. */
	struct process *parent;

	/* The pointer to the process page tree. */
	pageentry *pagetree;

	/* The address of the first unmapped page in the process virtual
	   address space. */
	uptr ceil;

	/* The context of the process in the trampoline. */
	u8 ctx[CTX_SIZE];

	/* The pointers to the previous and next processes. These are needed to
	   create linked lists of processes. */
	struct process *p;
	struct process *n;
};

/* It allocates process of state BLOCKED. If parent is not NULL, the allocated
   process is referred to as the "child" process. If parent is NULL, the
   allocated process is referred to as the "init" process. The init process
   may only be allocated once. On success, a pointer to the allocated process
   is returned. On failure, NULL is returned and the panic message is set. */
struct process *allocprocess(struct process *parent);

/* It frees process p and all its allocated memory. On success, 0 is returned.
   On failure, -1 is returned and the panic message is set. */
s8 freeprocess(struct process *p);

/* It increases the ceil of process p by a page, mapping it to the f
   physical frame and setting its options to opts. On success, 0 is returned
   and o is set to the virtual address of the mapped page if not NULL. On
   failure, -1 is returned and the panic message is set. */
s8 growprocess(uptr *o, struct process *p, void *f, struct pageoptions opts);

/* It returns the init process. If the init process has not been allocated yet,
   NULL is returned. */
struct process *initprocess(void);

/* It returns the head of the linked list of all the allocated processes. */
struct process *processes(void);

/* It returns the physical address of the trap frame of process p, or NULL if
   it is not mapped in the process page tree. NULL is returned if p is NULL. */
void *trapframe(struct process *p);

#endif
