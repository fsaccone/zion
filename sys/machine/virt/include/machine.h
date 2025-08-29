#ifndef _MACHINE_H
#define _MACHINE_H

#define RAM_SIZE (128 * 1024 * 1024)

#define KERNEL_START 0x80000000
#define KERNEL_SIZE  0xa0000

#define FREE_MEMORY_REGIONS 1
void *freememregions[FREE_MEMORY_REGIONS][2] = {
	/* start                       size */
	{  KERNEL_START + KERNEL_SIZE, RAM_SIZE - KERNEL_START - KERNEL_SIZE },
};

#define HAS_UART

#define UART0               (uint8_t *)(0x10000000)
#define UART_THR            (uint8_t *)(UART0 + 0x00)
#define UART_LSR            (uint8_t *)(UART0 + 0x05)
#define UART_LSR_EMPTY_MASK (0x40)

#endif
