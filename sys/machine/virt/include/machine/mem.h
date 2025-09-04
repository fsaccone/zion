#ifndef _MACHINE_MEM_H
#define _MACHINE_MEM_H

#include <types.h>

#define RAM_SIZE (4L * 1024L * 1024L * 1024L)

#define KERNEL_START (un)0x80000000
#define KERNEL_SIZE  (un)(256 * 1024 + 1 * 1024 * 1024)

#define FREE_MEMORY_REGIONS_LEN 1
#define FREE_MEMORY_REGIONS \
	/* start                       size                                   */ \
	{  KERNEL_START + KERNEL_SIZE, RAM_SIZE - (KERNEL_START + KERNEL_SIZE) },

#define UART0 (u8 *)(0x10000000)

#define CLINT (u8 *)(0x02000000)

#endif
