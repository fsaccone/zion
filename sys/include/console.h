#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <arch/types.h>

#define CONSOLE_WRITE_MAX 4096

/* It prints a message to serial console and returns the amount of printed
   characters. */
u16 consolewrite(char *m);

/* It prints an integer to serial console in binary form and returns the amount
   of printed characters. */
u16 consolewriteb2(u64 n);

/* It prints an integer to serial console in signed decimal form and returns
   the amount of printed characters. */
u16 consolewritebase10s(s64 n);

/* It prints an integer to serial console in unsigned decimal form and returns
   the amount of printed characters. */
u16 consolewriteb10u(u64 n);

/* It prints an integer to serial console in hexadecimal form and returns the
   amount of printed characters. */
u16 consolewriteb16(u64 n);

/* It initializes serial console. */
void initconsole(void);

#endif
