#ifndef _TAR_H
#define _TAR_H

#include <arch/types.h>

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

#endif
