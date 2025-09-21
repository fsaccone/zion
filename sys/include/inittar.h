#ifndef _INITTAR_H
#define _INITTAR_H

#include <arch/types.h>

/* At build time, the init.tar archive containing the init file system is
   embedded into the kernel binary right after this special magic sequence of
   bytes. It is important to not embed these bytes, in order, in a single
   structure, e.g. an array: the kernel may wrongly locate the init.tar archive
   at that location. */
#define INITTAR_MAGIC_BYTES_0  0x5a
#define INITTAR_MAGIC_BYTES_1  0x49
#define INITTAR_MAGIC_BYTES_2  0x4f
#define INITTAR_MAGIC_BYTES_3  0x4e
#define INITTAR_MAGIC_BYTES_4  0x49
#define INITTAR_MAGIC_BYTES_5  0x4e
#define INITTAR_MAGIC_BYTES_6  0x49
#define INITTAR_MAGIC_BYTES_7  0x54
#define INITTAR_MAGIC_BYTES_8  0x54
#define INITTAR_MAGIC_BYTES_9  0x41
#define INITTAR_MAGIC_BYTES_10 0x52
#define INITTAR_MAGIC_BYTES_11 0x53
#define INITTAR_MAGIC_BYTES_12 0x54
#define INITTAR_MAGIC_BYTES_13 0x41
#define INITTAR_MAGIC_BYTES_14 0x52
#define INITTAR_MAGIC_BYTES_15 0x54

#endif
