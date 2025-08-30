#ifndef _LOG_H
#define _LOG_H

/* Prints a message indented to the last debugheader */
void debug(char *m);

/* Prints a message prefixed with 'DEBUG: ' */
void debugheader(char *h);

/* Prints an address indented to the last debugheader */
void debugptr(void *ptr);

/* Prints an error message and panics */
void panic(char *m);

#endif
