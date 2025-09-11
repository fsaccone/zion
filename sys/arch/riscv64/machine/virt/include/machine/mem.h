#ifndef _MACHINE_MEM_H
#define _MACHINE_MEM_H

#include <arch/types.h>

#define RAM_SIZE (4L * 1024L * 1024L * 1024L)

#define KERNEL_START 0x80000000
/* This is hardcoded to guess the size of this codebase's binary */
#define KERNEL_SIZE  (256 * 1024)

#define FREE_MEMORY_REGIONS_LEN 1
#define FREE_MEMORY_REGIONS \
	/* start                       size                                   */ \
	{  KERNEL_START + KERNEL_SIZE, RAM_SIZE - (KERNEL_START + KERNEL_SIZE) },

#define UART0 0x10000000

#define CLINT 0x02000000

#define PLIC      0xc000000

#endif
