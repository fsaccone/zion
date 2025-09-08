#ifndef __SYSCALL_H
#define __SYSCALL_H

#include <arch/types.h>
#include <interrupt.h>

/* The first interrupt argument is used to identify the type of the syscall. */
#define SYSCALL_ARGS (INTERRUPT_ARGS - 1)

/* System calls */
#define SYSCALL_SHUTDOWN 0x0000
#define SYSCALL_REBOOT   0x0001

/* Handles system call of type type having arguments args. Returns 0 normally
   and -1 in case of error. */
int syscall(u16 type, un args[SYSCALL_ARGS]);

#endif
