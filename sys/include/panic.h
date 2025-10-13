#ifndef _PANIC_H
#define _PANIC_H

/* It prints the panic message and its call trace path, disable interrupts
   and loops infinitely. */
void panic(void);

/* It sets the panic message to m and resets the call trace path. */
void setpanicmsg(char *m);

/* It appends t to the panic message call trace path. */
void tracepanicmsg(char *t);

#endif
