#ifndef _PANIC_H
#define _PANIC_H

/* Prints an error message to serial console and panics. */
void panic(char *m);

/* Sets the additional message the next panic call will print to m. */
void setpanicmsg(char *m);

/* Appends m to the additional message of the next panic call formatted as a
   path of the call trace leading to the function that set the initial
   message. */
void tracepanicmsg(char *m);

#endif
