#ifndef _MACHINE_H
#define _MACHINE_H

#include <arch.h>

#define RAM_SIZE (4L * 1024L * 1024L * 1024L)

#define KERNEL_START (uintn_t)0x80000000
#define KERNEL_SIZE  (uintn_t)(256 * 1024 + 1 * 1024 * 1024)

#define FREE_MEMORY_REGIONS_LEN 1
#define FREE_MEMORY_REGIONS \
	/* start  size            */ \
	{  0,     KERNEL_START - 1 },

#define HAS_UART

#define UART0               (uint8_t *)(0x10000000)
#define UART_THR            (uint8_t *)(UART0 + 0x00)
#define UART_LSR            (uint8_t *)(UART0 + 0x05)
#define UART_LSR_EMPTY_MASK (0x40)

#endif
