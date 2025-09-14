#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <arch/types.h>

#define CONSOLE_WRITE_MAX 4096

/* Prints a message to serial console and returns the amount of printed
   characters. */
u16 consolewrite(char *m);

/* Prints an integer to serial console in binary form and returns the amount of
   printed characters. */
u16 consolewriteintb2(u64 n);

/* Prints an integer to serial console in signed decimal form and returns the
   amount of printed characters. */
u16 consolewriteintbase10s(s64 n);

/* Prints an integer to serial console in unsigned decimal form and returns the
   amount of printed characters. */
u16 consolewriteintb10u(u64 n);

/* Prints an integer to serial console in hexadecimal form and returns the
   amount of printed characters. */
u16 consolewriteintb16(u64 n);

#endif
