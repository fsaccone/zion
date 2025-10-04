#ifndef __INIT_H
#define __INIT_H

#include <tar.h>

/* Create the init process using the tar file header init to get the init
   file. */
s8 createinitprocess(struct tarheader *init);

#endif
