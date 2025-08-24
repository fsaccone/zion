#ifndef _ARCH_TYPES_H
#define _ARCH_TYPES_H

typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed int   int32_t;
typedef struct {
	int32_t low;
	int32_t high;
} int64_t;

typedef int32_t intn_t;

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef struct {
	uint32_t low;
	uint32_t high;
} uint64_t;

typedef uint32_t uintn_t;

typedef float  float32_t;
typedef double float64_t;

typedef uint32_t size_t;

#endif
