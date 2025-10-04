#ifndef __INITTAR_H
#define __INITTAR_H

#include <tar.h>

/* Returns an allocated struct tarnode linked list containing all the files in
   init.tar or NULL in case of error. */
struct tarnode *allocinittarfiles(void);

/* Frees all the allocated struct tarnodes of the files linked list. */
void freeinittarfiles(struct tarnode *files);

#endif
