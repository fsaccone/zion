#ifndef _ZION_SYSCALL_H
#define _ZION_SYSCALL_H

void exit(void);
int fork(void);
void *growmem(short s);
void shutdown(void);
void reboot(void);

#endif
