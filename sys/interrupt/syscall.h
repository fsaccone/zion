#ifndef __SYSCALL_H
#define __SYSCALL_H

#include <arch/types.h>
#include <interrupt.h>

/* The first interrupt argument is used to identify the type of the syscall. */
#define SYSCALL_ARGS (INTERRUPT_ARGS - 1)

/* Handles system call of type type having arguments args. */
void syscall(u8 type, un args[SYSCALL_ARGS]);

#endif
