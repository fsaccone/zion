#ifndef _USER_H
#define _USER_H

#include <arch/page.h>

/* Jumps to address a in user mode and switches to virtual memory using page
   tree ptree if it is not NULL. */
void usermode(void *a, pageentry *ptree);

#endif
