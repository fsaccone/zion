#ifndef _ARCH_PAGE_H
#define _ARCH_PAGE_H

#include <arch/types.h>

#define PAGE_SIZE_EXPONENT 12
#define PAGE_SIZE          (1 << PAGE_SIZE_EXPONENT)

#define PAGE_ENTRY_GET_VALID(e)    ((e) & 1)
#define PAGE_ENTRY_GET_PPN(e)      (((((e) >> 10) << 20) >> 10))
#define PAGE_ENTRY_GET_R(e)        (((e) >> 1) & 1)
#define PAGE_ENTRY_GET_W(e)        (((e) >> 2) & 1)
#define PAGE_ENTRY_GET_X(e)        (((e) >> 3) & 1)
#define PAGE_ENTRY_GET_WALKABLE(e) (~PAGE_ENTRY_GET_R(e) \
                                  & ~PAGE_ENTRY_GET_W(e) \
                                  & ~PAGE_ENTRY_GET_X(e))

#define PAGE_ENTRY_ADD_VALID(e)    ((e) | 1)
#define PAGE_ENTRY_ADD_R(e)        ((e) | (1 << 1))
#define PAGE_ENTRY_ADD_W(e)        ((e) | (1 << 2))
#define PAGE_ENTRY_ADD_X(e)        ((e) | (1 << 3))
#define PAGE_ENTRY_ADD_USER(e)     ((e) | (1 << 4))

#define PAGE_ENTRY_REM_VALID(e)    ((e) & ~1)
#define PAGE_ENTRY_REM_R(e)        ((e) & ~(1 << 1))
#define PAGE_ENTRY_REM_W(e)        ((e) & ~(1 << 2))
#define PAGE_ENTRY_REM_X(e)        ((e) & ~(1 << 3))
#define PAGE_ENTRY_REM_USER(e)     ((e) & ~(1 << 4))

#define PAGE_ENTRY_SET_PPN(e, ppn) ((e) | (ppn))
#define PAGE_ENTRY_SET_WALKABLE(e) (PAGE_ENTRY_REM_R(e) \
                                  | PAGE_ENTRY_REM_W(e) \
                                  | PAGE_ENTRY_REM_X(e))

typedef un pageentry;

#endif
