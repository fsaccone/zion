#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <arch/types.h>

/* The first argument register is used to identify the system call type and is
   not included here. */
#define SYSCALL_ARGS 4

/* The system call handler. System calls only come from user mode. */
void syscall(u16 code, ureg *args);

/* System call codes and their respective functions. */

/* Shuts down the machine. Does not return. */
#define SYSCALL_SHUTDOWN 0
void shutdown(void);

/* Reboots the machine. Does not return. */
#define SYSCALL_REBOOT 1
void reboot(void);

#endif
