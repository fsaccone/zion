#include <arch/serial.h>

void
serial_print(const char *str)
{
	while (*str) {
		while(!(*UART_LSR & UART_LSR_EMPTY_MASK));
		*UART_THR = *str++;
	}
}
