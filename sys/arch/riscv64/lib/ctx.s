.section .text
.global switchctx

switchctx:
	# If old is NULL, just load new
	beqz a0, 1f

	# Since ra is already set to the address that called switchctx, we do
	# not need to store the pc manually

	# Save all registers to old
	sd x1,  (0  * 8)(a0)
	sd x2,  (1  * 8)(a0)
	sd x3,  (2  * 8)(a0)
	sd x4,  (3  * 8)(a0)
	sd x5,  (4  * 8)(a0)
	sd x6,  (5  * 8)(a0)
	sd x7,  (6  * 8)(a0)
	sd x8,  (7  * 8)(a0)
	sd x9,  (8  * 8)(a0)
	sd x10, (9  * 8)(a0)
	sd x11, (10 * 8)(a0)
	sd x12, (11 * 8)(a0)
	sd x13, (12 * 8)(a0)
	sd x14, (13 * 8)(a0)
	sd x15, (14 * 8)(a0)
	sd x16, (15 * 8)(a0)
	sd x17, (16 * 8)(a0)
	sd x18, (17 * 8)(a0)
	sd x19, (18 * 8)(a0)
	sd x20, (19 * 8)(a0)
	sd x21, (20 * 8)(a0)
	sd x22, (21 * 8)(a0)
	sd x23, (22 * 8)(a0)
	sd x24, (23 * 8)(a0)
	sd x25, (24 * 8)(a0)
	sd x26, (25 * 8)(a0)
	sd x27, (26 * 8)(a0)
	sd x28, (27 * 8)(a0)
	sd x29, (28 * 8)(a0)
	sd x30, (29 * 8)(a0)
	sd x31, (30 * 8)(a0)

1:
	# Load all registers from new
	ld x1,  (0  * 8)(a1)
	ld x2,  (1  * 8)(a1)
	ld x3,  (2  * 8)(a1)
	ld x4,  (3  * 8)(a1)
	ld x5,  (4  * 8)(a1)
	ld x6,  (5  * 8)(a1)
	ld x7,  (6  * 8)(a1)
	ld x8,  (7  * 8)(a1)
	ld x9,  (8  * 8)(a1)
	ld x10, (9  * 8)(a1)
	ld x11, (10 * 8)(a1)
	ld x12, (11 * 8)(a1)
	ld x13, (12 * 8)(a1)
	ld x14, (13 * 8)(a1)
	ld x15, (14 * 8)(a1)
	ld x16, (15 * 8)(a1)
	ld x17, (16 * 8)(a1)
	ld x18, (17 * 8)(a1)
	ld x19, (18 * 8)(a1)
	ld x20, (19 * 8)(a1)
	ld x21, (20 * 8)(a1)
	ld x22, (21 * 8)(a1)
	ld x23, (22 * 8)(a1)
	ld x24, (23 * 8)(a1)
	ld x25, (24 * 8)(a1)
	ld x26, (25 * 8)(a1)
	ld x27, (26 * 8)(a1)
	ld x28, (27 * 8)(a1)
	ld x29, (28 * 8)(a1)
	ld x30, (29 * 8)(a1)
	ld x31, (30 * 8)(a1)

	# Jump to ra of new
	ret
