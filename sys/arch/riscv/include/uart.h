#ifndef _UART_H
#define _UART_H

#include <types.h>

#define UART0               (uint8_t *)(0x10000000)
#define UART_THR            (uint8_t *)(UART0)
#define UART_LSR            (uint8_t *)(UART0 + 0x05)
#define UART_LSR_EMPTY_MASK (0x40)

void uart_print(const char *str);
void uart_putc(char c);

#endif
