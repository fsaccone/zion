#ifndef _USER_H
#define _USER_H

#include <arch/page.h>

/* Returns the physical address of the user mode interrupt handler. All the
   handler does is switch to phyisical memory, reset the kernel interrupt
   handler and jump to interrupt, making it always fit inside a page. */
void *userinterruptbase(void);

/* Get virtual address pc of usermode for core c. */
uptr getuserpc(u16 c);

/* Get page tree of usermode for core c. */
pageentry *getuserptree(u16 c);

/* Get stack pointer of usermode for core c. */
uptr getusersp(u16 c);

/* Get trap frame of usermode for core c. */
void *getusertframe(u16 c);

/* Set virtual address pc as the one usermode jumps to for core c. */
void setuserpc(u16 c, uptr pc);

/* Set page tree ptree as the one used by usermode to enable virtual memory for
   core c. */
void setuserptree(u16 c, pageentry *ptree);

/* Set address sp as the stack pointer to use for usermode for core c. */
void setusersp(u16 c, uptr sp);

/* Set trap frame tframe as the one used by usermode to save the necessary
   data before switching mode for core c. */
void setusertframe(u16 c, void *tframe);

/* Sets up the environment to enable virtual memory and switch to user mode:
     1. Save the necessary data to the trap frame set by setusertframe.
     2. Enables virtual memory using the page tree set by setuserptree.
     3. Sets stack pointer to the address set by setusersp.
     4. Enables user mode.
     5. Set interrupt handler to virtual address 0x0.
     6. Jumps to the virtual address set by setuserpc. */
void usermode(void);

/* Returns the physical address of the usermode function. */
void *usermodebase(void);

#endif
