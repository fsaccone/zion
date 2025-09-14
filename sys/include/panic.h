#ifndef _PANIC_H
#define _PANIC_H

/* Prints the panic message along with its call trace path. */
void panic(void);

/* Sets the message of the next panic call to m. */
void setpanicmsg(char *m);

/* Appends t to the message of the next panic call formatted as a path of the
   call trace leading to the function that set the initial message. */
void tracepanicmsg(char *t);

#endif
