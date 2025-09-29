#ifndef _MACHINE_MEM_H
#define _MACHINE_MEM_H

#include <arch/types.h>
#include <config.h>

/* (4G = 4 * 1024 * 1024 * 1024 = 4294967296) */
#define RAM_SIZE (uptr)4294967296

#define KERNEL_START (uptr)0x80000000
#define KERNEL_SIZE  (uptr)BINARY_SIZE

#define FREE_MEMORY_REGIONS_LEN 1
#define FREE_MEMORY_REGIONS { \
	/* start                       end     */ \
	{  KERNEL_START + KERNEL_SIZE, RAM_SIZE }, \
}

#define PLIC (uptr)0xc000000

/* Device base addresses. */
#define UART0 (uptr)0x10000000

/* Device IRQs. */
#define UART0_IRQ 10

/* Device priorities. */
#define UART0_PRIORITY 1

#endif
