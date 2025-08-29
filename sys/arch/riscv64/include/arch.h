#ifndef _ARCH_H
#define _ARCH_H

#define BIT64

#define PAGE_SIZE_EXPONENT 12
#define PAGE_SIZE          (1 << PAGE_SIZE_EXPONENT)

typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed int   int32_t;
typedef signed long  int64_t;
typedef struct {
	int64_t low;
	int64_t high;
} int128_t;

typedef int64_t intn_t;

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long  uint64_t;
typedef struct {
	uint64_t low;
	uint64_t high;
} uint128_t;

typedef uint64_t uintn_t;

typedef float  float32_t;
typedef double float64_t;

typedef uint64_t size_t;

#endif
