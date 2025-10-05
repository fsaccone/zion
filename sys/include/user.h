#ifndef _USER_H
#define _USER_H

#include <arch/page.h>

/* Set virtual address pc as the one usermode jumps to. */
void setuserpc(uptr pc);

/* Set page tree ptree as the one used by usermode to enable virtual memory. */
void setuserptree(pageentry *ptree);

/* Enables virtual memory using the page tree set by setuserptree and jumps to
   address 0x0 in user mode. This is supposed to be set as the program counter
   of a context before switching to it. */
void usermode(void);

#endif
