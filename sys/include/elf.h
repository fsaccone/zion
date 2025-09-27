#ifndef _ELF_H
#define _ELF_H

#include <arch/page.h>

/* Loads the ELF file whose content is at elf to page table pt and returns its
   load virtual address. */
void *allocelf(void *elf, pageentry *pt[PAGE_TABLE_ENTRIES]);

#endif
