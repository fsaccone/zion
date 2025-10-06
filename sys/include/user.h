#ifndef _USER_H
#define _USER_H

#include <arch/page.h>

/* Get virtual address pc of usermode for core c. */
uptr getuserpc(u16 c);

/* Get page tree of usermode for core c. */
pageentry *getuserptree(u16 c);

/* Set virtual address pc as the one usermode jumps to for core c. */
void setuserpc(u16 c, uptr pc);

/* Set page tree ptree as the one used by usermode to enable virtual memory for
   core c. */
void setuserptree(u16 c, pageentry *ptree);

/* Enables virtual memory using the page tree set by setuserptree and jumps to
   address set by seruserpc in user mode. This is supposed to be set as the
   program counter of a context before switching to it. */
void usermode(void);

#endif
