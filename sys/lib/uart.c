#include <uart.h>

#include <arch/uart.h>

void
uart_print(const char *str)
{
	while (*str) uart_putc(*str++);
}

void
uart_putc(char c)
{
	while(!(*UART_LSR & UART_LSR_EMPTY_MASK));
	*UART_THR = c;
}
