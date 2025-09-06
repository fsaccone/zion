.section .text
.global switchctx

switchctx:
	# If old is NULL, just load new
	beqz a0, 1f

	# Since ra is already set to the address that called switchctx, we do
	# not need to store the pc manually

	# Save all registers to old
	sd x1,  0(a0)
	sd x2,  8(a0)
	sd x3,  16(a0)
	sd x4,  24(a0)
	sd x5,  32(a0)
	sd x6,  40(a0)
	sd x7,  48(a0)
	sd x8,  56(a0)
	sd x9,  64(a0)
	sd x10, 72(a0)
	sd x11, 80(a0)
	sd x12, 88(a0)
	sd x13, 96(a0)
	sd x14, 104(a0)
	sd x15, 112(a0)
	sd x16, 120(a0)
	sd x17, 128(a0)
	sd x18, 136(a0)
	sd x19, 144(a0)
	sd x20, 152(a0)
	sd x21, 160(a0)
	sd x22, 168(a0)
	sd x23, 176(a0)
	sd x24, 184(a0)
	sd x25, 192(a0)
	sd x26, 200(a0)
	sd x27, 208(a0)
	sd x28, 216(a0)
	sd x29, 224(a0)
	sd x30, 232(a0)
	sd x31, 240(a0)

1:
	# Load all registers from new
	ld x1,  0(a1)
	ld x2,  8(a1)
	ld x3,  16(a1)
	ld x4,  24(a1)
	ld x5,  32(a1)
	ld x6,  40(a1)
	ld x7,  48(a1)
	ld x8,  56(a1)
	ld x9,  64(a1)
	ld x10, 72(a1)
	ld x11, 80(a1)
	ld x12, 88(a1)
	ld x13, 96(a1)
	ld x14, 104(a1)
	ld x15, 112(a1)
	ld x16, 120(a1)
	ld x17, 128(a1)
	ld x18, 136(a1)
	ld x19, 144(a1)
	ld x20, 152(a1)
	ld x21, 160(a1)
	ld x22, 168(a1)
	ld x23, 176(a1)
	ld x24, 184(a1)
	ld x25, 192(a1)
	ld x26, 200(a1)
	ld x27, 208(a1)
	ld x28, 216(a1)
	ld x29, 224(a1)
	ld x30, 232(a1)
	ld x31, 240(a1)

	# Jump to ra of new
	ret
