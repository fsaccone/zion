#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <arch/types.h>
#include <interrupt.h>

/* The first interrupt argument is used to identify the system call type. */
#define SYSCALL_ARGS (INTERRUPT_ARGS - 1)

/* System call codes. */
#define SYSCALL_SHUTDOWN 0
#define SYSCALL_REBOOT   1

/* The system call handler. */
void syscall(u16 type, ureg *args);

#endif
