#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <arch/types.h>

/* The number of argument registers taken into account as the system call
   arguments. */
#define SYSCALL_ARGS 4

/* It handles system call code of arguments args. */
void syscall(u16 code, ureg *args);

/* It creates a new process by duplicating the calling process. On failure, -1
   is returned in the parent and and no child process is created. On success,
   the PID of the child process is returned in the parent, and 0 is returned in
   the child. */
#define SYSCALL_FORK 0
void fork(void);

/* It grows process memory by how many pages are needed to fit size s. Each new
   page is mapped to a newly allocated frame. It uses urw-a as the options of
   the new pages. On success, the address of the first allocated page is
   returned in the process. On failure, NULL is returned in the process. */
#define SYSCALL_GROWMEM 1
void growmem(uptr s);

/* It shuts down the machine. */
#define SYSCALL_SHUTDOWN 2
void shutdown(void);

/* It reboots the machine. */
#define SYSCALL_REBOOT 3
void reboot(void);

#endif
