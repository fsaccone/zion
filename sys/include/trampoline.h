#ifndef _TRAMPOLINE_H
#define _TRAMPOLINE_H

#include <arch/page.h>

/* Prepares the kernel trap frame and context to jump to trampolineret, to make
   it do:
     - Use page tree ptree.
     - Use interrupt handler interrupt.
     - Jump to program counter pc. */
void setuptrampolineret(pageentry *ptree, void *interrupt, void *pc);

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
void trampolineret(void);

/* Returns the virtual address of trampolineret in a virtual address space. */
void *trampolineretbase(void);

#endif
