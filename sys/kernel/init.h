#ifndef __INIT_H
#define __INIT_H

#include <tar.h>

/* Create the init process using the tar file header init to get the init
   file. */
s8 createinitprocess(struct tarheader *init);

/* Find and return the init file in files, or NULL if no such file is
   present. */
struct tarheader *findinitfile(struct tarnode *files);

#endif
