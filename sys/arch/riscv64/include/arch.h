#ifndef _ARCH_H
#define _ARCH_H

#define BIT64

#define PAGE_SIZE_EXPONENT 12
#define PAGE_SIZE          (1 << PAGE_SIZE_EXPONENT)

typedef signed char  s8;
typedef signed short s16;
typedef signed int   s32;
typedef signed long  s64;

typedef s64 sn;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

typedef u64 un;

#endif
