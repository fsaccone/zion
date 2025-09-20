#ifndef _INITTAR_H
#define _INITTAR_H

#include <arch/types.h>

/* At build time, the init.tar archive containing the init file system is
   embedded into the kernel binary right after this special magic sequence of
   bytes. */
#define INITTAR_MAGIC_BYTES { \
	0x5A, 0x49, 0x4F, 0x4E, \
	0x49, 0x4E, 0x49, 0x54, \
	0x54, 0x41, 0x52, 0x53, \
	0x54, 0x41, 0x52, 0x54, \
}
#define INITTAR_MAGIC_BYTES_LENGTH 16

#endif
