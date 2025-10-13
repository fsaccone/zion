#ifndef _MATH_H
#define _MATH_H

/* It returns the value of bit n of bitmap bm. */
#define BITMAPGET(bm, n) (((bm)[(n) / 8] >> ((n) % 8)) & 1)

/* It sets the value of bit n of bitmap bm to 1. */
#define BITMAPADD(bm, n) ((bm)[(n) / 8] |= (1 << ((n) % 8)))

/* It sets the value of bit n of bitmap bm to 0. */
#define BITMAPREM(bm, n) ((bm)[(n) / 8] &= ~(1 << ((n) % 8)))

/* It returns the greatest integer multiple of n that is less than or equal to
   x. */
#define FLOOR(x, n) (((x) / (n)) * (n))

/* It returns the smallest integer multiple of n that is greater than or equal
   to x. */
#define CEIL(x, n)  ((((x) + (n) - 1) / (n)) * (n))

/* It returns the larger of the two values a and b. */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/* It returns the smaller of the two values a and b. */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#endif
