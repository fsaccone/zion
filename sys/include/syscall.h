#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <arch/types.h>

/* The first argument register is used to identify the system call type and is
   not included here. */
#define SYSCALL_ARGS 4

/* The system call handler. System calls only come from user mode. */
void syscall(u16 code, ureg *args);

/* System call codes and their respective functions. */

/* Creates a new process by duplicating the calling process. On failure, -1 is
   returned in the parent and and no child process is created. On success, the
   PID of the child process is returned in the parent, and 0 is returned in the
   child. */
#define SYSCALL_FORK 0
void fork(void);

/* Grows process memory by how many pages are needed to fit size s, allocating
   frames in physical memory. It uses urw-a as the permission of the new pages.
   It returns the address of the first allocated page or NULL in case of
   error. */
#define SYSCALL_GROWMEM 1
void *growmem(uptr s);

/* Shuts down the machine. Does not return. */
#define SYSCALL_SHUTDOWN 2
void shutdown(void);

/* Reboots the machine. Does not return. */
#define SYSCALL_REBOOT 3
void reboot(void);

#endif
