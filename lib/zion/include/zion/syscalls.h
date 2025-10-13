#ifndef _ZION_SYSCALLS_H
#define _ZION_SYSCALLS_H

void exit(void);
int fork(void);
void *growmem(short s);
void shutdown(void);
void reboot(void);

#endif
