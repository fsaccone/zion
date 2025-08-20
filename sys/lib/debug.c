#include <debug.h>

#include <arch/uart.h>

void
debug(const char *str)
{
	while (*str) {
		while(!(*UART_LSR & UART_LSR_EMPTY_MASK));
		*UART_THR = *str++;
	}
}
