#ifndef _ELF_H
#define _ELF_H

#include <arch/page.h>

struct elffileflags {
	/* Read permission. */
	u8 r : 1;

	/* Write permission. */
	u8 w : 1;

	/* Execute permission. */
	u8 x : 1;

	u8 reserved0 : 1;
	u8 reserved1[3];
};

struct elfprogflags {
	u8 reserved[4];
};

/* Loads the ELF file whose content is at elf to a newly allocated page tree at
   address 0x0 and returns the address to the page tree or NULL in case of
   error. */
pageentry *allocelf(void *elf);

#endif
