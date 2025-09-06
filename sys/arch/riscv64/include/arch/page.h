#ifndef _ARCH_PAGE_H
#define _ARCH_PAGE_H

#define PAGE_SIZE_EXPONENT 12
#define PAGE_SIZE          (1 << PAGE_SIZE_EXPONENT)

#define PAGE_ENTRY_VALID(e) ((e) & 1)
#define PAGE_ENTRY_PPN(e)   (((((e) >> 10) << 20) >> 10))

#endif
