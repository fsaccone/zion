#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <arch/types.h>

/* The first argument register is used to identify the system call type and is
   not included here. */
#define SYSCALL_ARGS 4

/* System call codes. */
#define SYSCALL_SHUTDOWN 0
#define SYSCALL_REBOOT   1

/* The system call handler. System calls only come from user mode. */
void syscall(u16 type, ureg *args);

#endif
