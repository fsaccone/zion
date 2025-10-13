#ifndef _TRAMPOLINE_H
#define _TRAMPOLINE_H

#include <arch/page.h>
#include <arch/types.h>

/* It copies the context saved in trap frame s to trap frame d. */
void copytrapframe(void *d, void *s);

/* It initializes trap frame tframe using startpc as the initial program
   counter and trampoline as trampoline page address. */
void inittrapframe(void *tframe, uptr startpc, uptr trampoline);

/* It prepares trap frame tframe, mapped in the ptree page tree at address
   PROC_VAS_TRAP_FRAME, to jump to usermode. It also configures the context of
   the calling core. It is important to run this right before the jump to
   usermode. */
void preparetrapframe(void *tframe, pageentry *ptree);

/* It sets the return value of trap frame tframe to r. It does nothing if
   tframe is NULL. */
void setreturn(void *tframe, ureg r);

/* It is aligned to PAGE_SIZE and fits inside a frame: it should be mapped to
   the same address in both kernel and user virtual address spaces, so that the
   page tree switch in it is possible. It makes use of the trap frame at
   address PROC_VAS_TRAP_FRAME, defined in process.h, doing:
     1. Save the context.
     2. Load the kernel essential data, such as the stack pointer and page
        tree.
     3. Call interrupt, which is now available in the kernel virtual address
        space.
     4. Switch back to the old page tree.
     5. Load the initial context and return to user mode. */
void trampoline(void);

/* It returns the physical address of trampoline. */
void *trampolinebase(void);

/* It is part of trampoline: it approximately resides at step 4. Before
   switching here, it is important to call preparetrapframe passing the page
   tree to switch to. */
void usermode(void);

/* It returns the virtual address of usermode provided that the virtual address
   of the trampoline page in the same virtual address space is trampoline. */
void *usermodebase(uptr trampoline);

#endif
