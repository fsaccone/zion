#ifndef _MACHINE_H
#define _MACHINE_H

#define RAM_SIZE 128 * 1024 * 1024

#define HAS_UART

#define UART0               (uint8_t *)(0x10000000)
#define UART_THR            (uint8_t *)(UART0 + 0x00)
#define UART_LSR            (uint8_t *)(UART0 + 0x05)
#define UART_LSR_EMPTY_MASK (0x40)

#endif
