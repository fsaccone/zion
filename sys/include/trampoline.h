#ifndef _TRAMPOLINE_H
#define _TRAMPOLINE_H

#include <arch/page.h>
#include <arch/types.h>

/* Initializes trap frame tframe using startpc as the virtual address of the
   initial program counter and trampoline as the virtual address of the
   trampoline page. */
void inittrapframe(void *tframe, uptr startpc, uptr trampoline);

/* Prepares trap frame tframe and the current context for a jump to usermode,
   using user page tree ptree. It is important to run this right before the
   jump to usermode. */
void preparetrapframe(void *tframe, pageentry *ptree);

/* Sets the return value of trap frame tframe to r. */
void setreturn(void *tframe, ureg r);

/* The trampoline. It is aligned to PAGE_SIZE and fits inside a page, to make
   it easily mappable in virtual address spaces: it should be mapped to the
   same address in both kernel and user virtual address spaces, so that the
   page tree switch may not break interrupts. It makes use of the trap frame at
   address VADDR_TRAP_FRAME, doing:
     1. Save the context.
     2. Load the kernel essential data, such as the stack pointer and page
        tree.
     3. Call interrupt, which is now available in the kernel virtual address
        space.
     4. Switch back to the old page tree.
     5. Load the initial context and return to user mode. */
void trampoline(void);

/* Returns the physical address of trampoline. */
void *trampolinebase(void);

/* Part of trampoline. Starts right after the interrupt handler returns,
   marking the return to the user code. */
void usermode(void);

/* Returns the virtual address of usermode given that the virtual address of
   the trampoline page is trampoline. */
void *usermodebase(uptr trampoline);

#endif
