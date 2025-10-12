#ifndef _ARCH_PAGE_H
#define _ARCH_PAGE_H

#include <arch/types.h>

/* The used scheme is Sv57. */

#define PAGE_TABLE_ENTRIES 512
#define PAGE_TABLE_LEVELS  5

#define PAGE_SIZE_EXPONENT 12
#define PAGE_SIZE          (1 << PAGE_SIZE_EXPONENT)

#define PAGE_ENTRY_GET_VALID(e)    ((e) & 1)
#define PAGE_ENTRY_GET_PADDR(e)    ((((e) >> 10) << 12))
#define PAGE_ENTRY_GET_R(e)        (((e) >> 1) & 1)
#define PAGE_ENTRY_GET_W(e)        (((e) >> 2) & 1)
#define PAGE_ENTRY_GET_X(e)        (((e) >> 3) & 1)
#define PAGE_ENTRY_GET_WALKABLE(e) (PAGE_ENTRY_GET_VALID(e) \
                                  & ~PAGE_ENTRY_GET_R(e) \
                                  & ~PAGE_ENTRY_GET_W(e) \
                                  & ~PAGE_ENTRY_GET_X(e))

#define PAGE_ENTRY_ADD_VALID(e) ((e) | 1)
#define PAGE_ENTRY_ADD_U(e)     ((e) | (1 << 4))
#define PAGE_ENTRY_ADD_R(e)     ((e) | (1 << 1))
#define PAGE_ENTRY_ADD_W(e)     ((e) | (1 << 2))
#define PAGE_ENTRY_ADD_X(e)     ((e) | (1 << 3))

#define PAGE_ENTRY_REM_VALID(e) ((e) & ~1)
#define PAGE_ENTRY_REM_U(e)     ((e) & ~(1 << 4))
#define PAGE_ENTRY_REM_R(e)     ((e) & ~(1 << 1))
#define PAGE_ENTRY_REM_W(e)     ((e) & ~(1 << 2))
#define PAGE_ENTRY_REM_X(e)     ((e) & ~(1 << 3))

#define PAGE_ENTRY_SET_PADDR(e, paddr) ((e) | (((paddr) >> 12) << 10))
#define PAGE_ENTRY_SET_WALKABLE(e)     (PAGE_ENTRY_ADD_VALID(e) \
                                      | PAGE_ENTRY_REM_R(e) \
                                      | PAGE_ENTRY_REM_W(e) \
                                      | PAGE_ENTRY_REM_X(e))

#define PAGE_LVLIDXS_FROM_VADDR(vaddr) { \
	((vaddr) >> 48) & 0b111111111, \
	((vaddr) >> 39) & 0b111111111, \
	((vaddr) >> 30) & 0b111111111, \
	((vaddr) >> 21) & 0b111111111, \
	((vaddr) >> 12) & 0b111111111, \
}

#define PAGE_VADDR_FROM_LVLIDXS(lvlidxs) (((lvlidxs)[0] << 48) \
                                        | ((lvlidxs)[1] << 39) \
                                        | ((lvlidxs)[2] << 30) \
                                        | ((lvlidxs)[3] << 21) \
                                        | ((lvlidxs)[4] << 12))

#define PAGE_VADDR_MAX ((1L << 57) - 1)

typedef uptr pageentry;

#endif
