#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <arch/types.h>

/* The first argument register is used to identify the system call type and is
   not included here. */
#define SYSCALL_ARGS 4

/* The system call handler. System calls only come from user mode. */
void syscall(u16 code, ureg *args);

/* System call codes and their respective functions. */

/* Grows process memory by how many pages are needed to fit size s, allocating
   frames in physical memory. It uses urw- as the permission of the new pages.
   It returns the address of the first allocated page or NULL in case of
   error. */
#define SYSCALL_GROWMEM 0
void *growmem(uptr s);

/* Shuts down the machine. Does not return. */
#define SYSCALL_SHUTDOWN 1
void shutdown(void);

/* Reboots the machine. Does not return. */
#define SYSCALL_REBOOT 2
void reboot(void);

#endif
