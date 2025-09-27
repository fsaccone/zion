#ifndef _ELF_H
#define _ELF_H

/* Loads the ELF file whose content is at elf to memory and returns its load
   address. */
void *allocelf(void *elf);

#endif
