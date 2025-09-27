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

/* Loads the ELF file whose content is at elf to page table pt and returns its
   load virtual address or NULL in case of error. */
void *allocelf(void *elf, struct elffileflags flags,
               pageentry *pt[PAGE_TABLE_ENTRIES]);

#endif
