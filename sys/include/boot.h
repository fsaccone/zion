/* ARCHITECTURE-SPECIFIC */
#ifndef _BOOT_H
#define _BOOT_H

/* Clears the bss section of the kernel code */
void clearbss(void);

/* Sets all general-purpose registers to 0 */
void clearregs(void);

#endif
