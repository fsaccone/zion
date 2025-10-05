#ifndef _UART_H
#define _UART_H

#include <arch/types.h>

/* Initializes the UART device controller. */
void inituart(void);

/* Writes character c to the UART controller. */
void uartwrite(u8 c);

#endif
