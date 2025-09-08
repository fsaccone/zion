#ifndef _LOG_H
#define _LOG_H

#include <arch/types.h>

#define LOG_MAX 4096

/* Prints a message. */
void debug(char *m);

/* Prints an integer in binary form. */
void debugintbase2(u64 n);

/* Prints an integer in signed decimal form. */
void debugintbase10s(s64 n);

/* Prints an integer in unsigned decimal form. */
void debugintbase10u(u64 n);

/* Prints an integer in hexadecimal form. */
void debugintbase16(u64 n);

/* Prints an error message and panics. */
void panic(char *m);

/* Sets the additional message the next panic call will print to m. */
void setpanicmsg(char *m);

/* Appends m to the additional message of the next panic call formatted as a
   path of the call trace leading to the function that set the initial
   message. */
void tracepanicmsg(char *m);

#endif
