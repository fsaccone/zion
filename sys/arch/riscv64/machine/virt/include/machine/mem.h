#ifndef _MACHINE_MEM_H
#define _MACHINE_MEM_H

#include <arch/types.h>
#include <config.h>

#define PHY_MEM_START (uptr)0x80000000
/* (4G = 4 * 1024 * 1024 * 1024 = 4294967296) */
#define PHY_MEM_SIZE  (uptr)4294967296
#define PHY_MEM_END   (PHY_MEM_START + PHY_MEM_SIZE)

#define KERNEL_START PHY_MEM_START
#define KERNEL_SIZE  (uptr)BINARY_SIZE
#define KERNEL_END   (KERNEL_START + KERNEL_SIZE)

#define RAMINIT_START (KERNEL_START + KERNEL_SIZE)
#define RAMINIT_SIZE  (uptr)RAMINIT_BINARY_SIZE
#define RAMINIT_END   (RAMINIT_START + RAMINIT_SIZE)

#define FREE_MEMORY_REGIONS_LEN 1
#define FREE_MEMORY_REGIONS { \
	/* start        end        */ \
	{  RAMINIT_END, PHY_MEM_END }, \
}

#define PLIC (uptr)0xc000000

/* Device base addresses. */
#define UART0 (uptr)0x10000000

/* Device IRQs. */
#define UART0_IRQ 10

/* Device priorities. */
#define UART0_PRIORITY 1

#endif
