#ifndef _ARCH_H
#define _ARCH_H

#define BIT64

#define PAGE_SIZE_EXPONENT 12
#define PAGE_SIZE          (1 << PAGE_SIZE_EXPONENT)

typedef signed char  int8;
typedef signed short int16;
typedef signed int   int32;
typedef signed long  int64;
typedef struct {
	int64 low;
	int64 high;
} int128;

typedef int64 intn;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned long  uint64;
typedef struct {
	uint64 low;
	uint64 high;
} uint128;

typedef uint64 uintn;

typedef uint64 size;

#endif
