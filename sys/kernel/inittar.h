#ifndef __INITTAR_H
#define __INITTAR_H

#include <tar.h>

/* Node of linked list of struct tarheader. */
struct tarnode {
	struct tarheader *h;
	struct tarnode   *n;
};

/* Returns an allocated struct tarnode linked list containing all the files in
   init.tar or NULL if init.tar is not found in the kernel memory space or it
   is empty. */
struct tarnode *allocinittarfiles(void);

/* Frees all the allocated struct tarnodes of the files linked list. */
void freeinittarfiles(struct tarnode *files);

#endif
