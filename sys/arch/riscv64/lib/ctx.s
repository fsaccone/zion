.section .text
.global setctxpc
.global switchctx

setctxpc:
	# Save new pc to ra of ctx.
	sd a1, (0 * 8)(a0)

	ret

switchctx:
	# Since ra is already set to the address that called switchctx, we do
	# not need to store the pc manually.

	# Save registers to old.
	sd ra,  (0  * 8)(a0)
	sd sp,  (1  * 8)(a0)
	sd gp,  (2  * 8)(a0)
	sd tp,  (3  * 8)(a0)
	sd s0,  (4  * 8)(a0)
	sd s1,  (5  * 8)(a0)
	sd s2,  (6  * 8)(a0)
	sd s3,  (7  * 8)(a0)
	sd s4,  (8  * 8)(a0)
	sd s5,  (9  * 8)(a0)
	sd s6,  (10 * 8)(a0)
	sd s7,  (11 * 8)(a0)
	sd s8,  (12 * 8)(a0)
	sd s9,  (13 * 8)(a0)
	sd s10, (14 * 8)(a0)
	sd s11, (15 * 8)(a0)

	# Load registers from new.
	ld ra,  (0  * 8)(a1)
	ld sp,  (1  * 8)(a1)
	ld gp,  (2  * 8)(a1)
	ld tp,  (3  * 8)(a1)
	ld s0,  (4  * 8)(a1)
	ld s1,  (5  * 8)(a1)
	ld s2,  (6  * 8)(a1)
	ld s3,  (7  * 8)(a1)
	ld s4,  (8  * 8)(a1)
	ld s5,  (9  * 8)(a1)
	ld s6,  (10 * 8)(a1)
	ld s7,  (11 * 8)(a1)
	ld s8,  (12 * 8)(a1)
	ld s9,  (13 * 8)(a1)
	ld s10, (14 * 8)(a1)
	ld s11, (15 * 8)(a1)

	# Jump to ra of new.
	ret
