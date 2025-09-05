.section .text
.global switchctx

switchctx:
	# If old is NULL, just load new
	beqz a0, 1f

	# Since ra is already set to the address that called switchctx, we do
	# not need to store the pc manually

	# Save all registers to old
	sw x1,  0(a0)
	sw x2,  8(a0)
	sw x3,  16(a0)
	sw x4,  24(a0)
	sw x5,  32(a0)
	sw x6,  40(a0)
	sw x7,  48(a0)
	sw x8,  56(a0)
	sw x9,  64(a0)
	sw x10, 72(a0)
	sw x11, 80(a0)
	sw x12, 88(a0)
	sw x13, 96(a0)
	sw x14, 104(a0)
	sw x15, 112(a0)
	sw x16, 120(a0)
	sw x17, 128(a0)
	sw x18, 136(a0)
	sw x19, 144(a0)
	sw x20, 152(a0)
	sw x21, 160(a0)
	sw x22, 168(a0)
	sw x23, 176(a0)
	sw x24, 184(a0)
	sw x25, 192(a0)
	sw x26, 200(a0)
	sw x27, 208(a0)
	sw x28, 216(a0)
	sw x29, 224(a0)
	sw x30, 232(a0)
	sw x31, 240(a0)

1:
	# Load all registers from new
	lw x1,  0(a1)
	lw x2,  8(a1)
	lw x3,  16(a1)
	lw x4,  24(a1)
	lw x5,  32(a1)
	lw x6,  40(a1)
	lw x7,  48(a1)
	lw x8,  56(a1)
	lw x9,  64(a1)
	lw x10, 72(a1)
	lw x11, 80(a1)
	lw x12, 88(a1)
	lw x13, 96(a1)
	lw x14, 104(a1)
	lw x15, 112(a1)
	lw x16, 120(a1)
	lw x17, 128(a1)
	lw x18, 136(a1)
	lw x19, 144(a1)
	lw x20, 152(a1)
	lw x21, 160(a1)
	lw x22, 168(a1)
	lw x23, 176(a1)
	lw x24, 184(a1)
	lw x25, 192(a1)
	lw x26, 200(a1)
	lw x27, 208(a1)
	lw x28, 216(a1)
	lw x29, 224(a1)
	lw x30, 232(a1)
	lw x31, 240(a1)

	# Jump to ra of new
	ret
