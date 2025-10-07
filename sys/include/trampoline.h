#ifndef _TRAMPOLINE_H
#define _TRAMPOLINE_H

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

#endif
