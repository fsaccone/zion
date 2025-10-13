#ifndef _UART_H
#define _UART_H

#include <arch/types.h>

/* It initializes the UART device controller. */
void inituart(void);

/* It writes the c character to the UART controller. */
void uartwrite(u8 c);

#endif
