#ifndef _LOG_H
#define _LOG_H

#include <arch/types.h>

/* Prints a message indented to the last debugheader */
void debug(char *m);

/* Prints a message prefixed with 'DEBUG: ' */
void debugheader(char *h);

/* Prints an integer in hexadecimal form */
void debugintbase16(u64 n);

/* Prints an error message and panics */
void panic(char *m);

#endif
