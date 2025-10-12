#ifndef _MATH_H
#define _MATH_H

/* Round integer x to its previous/next multiple of n. */
#define FLOOR(x, n) (((x) / (n)) * (n))
#define CEIL(x, n)  ((((x) + (n) - 1) / (n)) * (n))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#endif
