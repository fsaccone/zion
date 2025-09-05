#ifndef _USER_H
#define _USER_H

/* Jumps to function cb in user mode */
void usermode(void (*cb)(void));

#endif
