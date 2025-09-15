#ifndef _MACHINE_MEM_H
#define _MACHINE_MEM_H

#include <arch/types.h>

#define RAM_SIZE (4L * 1024L * 1024L * 1024L)

#define KERNEL_START 0x80000000
/* This is hardcoded to guess the size of this codebase's binary. */
#define KERNEL_SIZE  (256 * 1024)

#define FREE_MEMORY_REGIONS_LEN 1
#define FREE_MEMORY_REGIONS \
	/* start                       end     */ \
	{  KERNEL_START + KERNEL_SIZE, RAM_SIZE },

#define PLIC 0xc000000

/* Device base addresses. */
#define UART0 0x10000000

/* Device IRQs. */
#define UART0_IRQ 10

/* Device priorities. */
#define UART0_PRIORITY 1

#endif
