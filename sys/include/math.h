#ifndef _MATH_H
#define _MATH_H

/* Returns the value of bit n of bitmap bm. */
#define BITMAPGET(bm, n) (((bm)[(n) / 8] >> ((n) % 8)) & 1)

/* Sets the value of bit n of bitmap bm to 1/0. */
#define BITMAPADD(bm, n) ((bm)[(n) / 8] |= (1 << ((n) % 8)))
#define BITMAPREM(bm, n) ((bm)[(n) / 8] &= ~(1 << ((n) % 8)))

/* Round integer x to its previous/next multiple of n. */
#define FLOOR(x, n) (((x) / (n)) * (n))
#define CEIL(x, n)  ((((x) + (n) - 1) / (n)) * (n))

/* Returns the maximum/minimum between a and b. */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#endif
