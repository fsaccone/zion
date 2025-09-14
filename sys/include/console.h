#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <arch/types.h>

#define CONSOLE_WRITE_MAX 4096

/* Prints a message to serial console. */
void consolewrite(char *m);

/* Prints an integer to serial console in binary form. */
void consolewriteintb2(u64 n);

/* Prints an integer to serial console in signed decimal form. */
void consolewriteintbase10s(s64 n);

/* Prints an integer to serial console in unsigned decimal form. */
void consolewriteintb10u(u64 n);

/* Prints an integer to serial console in hexadecimal form. */
void consolewriteintb16(u64 n);

/* Prints an error message to serial console and panics. */
void panic(char *m);

/* Sets the additional message the next panic call will print to m. */
void setpanicmsg(char *m);

/* Appends m to the additional message of the next panic call formatted as a
   path of the call trace leading to the function that set the initial
   message. */
void tracepanicmsg(char *m);

#endif
