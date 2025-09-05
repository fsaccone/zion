#ifndef _LOG_H
#define _LOG_H

#include <arch/types.h>

/* Prints a message */
void debug(char *m);

/* Prints an integer in binary form */
void debugintbase2(u64 n);

/* Prints an integer in signed decimal form */
void debugintbase10s(s64 n);

/* Prints an integer in unsigned decimal form */
void debugintbase10u(u64 n);

/* Prints an integer in hexadecimal form */
void debugintbase16(u64 n);

/* Prints an error message and panics */
void panic(char *m);

#endif
