#ifndef _TAR_H
#define _TAR_H

#include <arch/types.h>

#define TAR_TYPE_AREG      '\0'
#define TAR_TYPE_REG       '0'
#define TAR_TYPE_HARDLINK  '1'
#define TAR_TYPE_SYMLINK   '2'
#define TAR_TYPE_CHARDEV   '3'
#define TAR_TYPE_BLOCKDEV  '4'
#define TAR_TYPE_DIRECTORY '5'
#define TAR_TYPE_FIFO      '6'
#define TAR_TYPE_RESERVED  '7'

struct tarheader {
	u8 name[100];
	u8 mode[8];
	u8 uid[8];
	u8 gid[8];
	u8 size[12];
	u8 mtime[12];
	u8 checksum[8];
	u8 type;
	u8 linkname[100];
	u8 magic[6];
	u8 version[2];
	u8 uname[32];
	u8 gname[32];
	u8 major[8];
	u8 minor[8];
	u8 prefix[155];

	/* Fill up to 512 bytes. */
	u8 reserved[12];
};

/* Returns 1 if ptr points to a valid tar header or 0 otherwise. */
u8 istarheader(void *ptr);

/* Returns the address of the program code of the tar header h. */
void *tarprogram(struct tarheader *h);

/* Returns the size of the file whose tar header is h. */
uptr tarsize(struct tarheader *h);

#endif
