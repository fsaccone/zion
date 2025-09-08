#ifndef _MACHINE_SYS_H
#define _MACHINE_SYS_H

#define DO_SYS_REBOOT   (*(u32 *)0x100000 = 0x7777)
#define DO_SYS_SHUTDOWN (*(u32 *)0x100000 = 0x5555)

#endif
