.section .text
.globl switch_ctx
.align 4

switch_ctx:
	beqz a0, load

	# skip ra
	# sw x1, 0(a0)
	sw x2, 4(a0)
	sw x3, 8(a0)
	sw x4, 12(a0)
	sw x5, 16(a0)
	sw x6, 20(a0)
	sw x7, 24(a0)
	sw x8, 28(a0)
	sw x9, 32(a0)
	# skip a0 and 01
	# sw x10, 36(a0)
	# sw x11, 40(a0)
	sw x12, 44(a0)
	sw x13, 48(a0)
	sw x14, 52(a0)
	sw x15, 56(a0)
	sw x16, 60(a0)
	sw x17, 64(a0)
	sw x18, 68(a0)
	sw x19, 72(a0)
	sw x20, 76(a0)
	sw x21, 80(a0)
	sw x22, 84(a0)
	sw x23, 88(a0)
	sw x24, 92(a0)
	sw x25, 96(a0)
	sw x26, 100(a0)
	sw x27, 104(a0)
	sw x28, 108(a0)
	sw x29, 112(a0)
	sw x30, 116(a0)
	sw x31, 120(a0)

load:
	# skip ra
	# lw x1, 0(a1)
	lw x2, 4(a1)
	lw x3, 8(a1)
	lw x4, 12(a1)
	lw x5, 16(a1)
	lw x6, 20(a1)
	lw x7, 24(a1)
	lw x8, 28(a1)
	lw x9, 32(a1)
	# skip a0 and a1
	# lw x10, 36(a1)
	# lw x11, 40(a1)
	lw x12, 44(a1)
	lw x13, 48(a1)
	lw x14, 52(a1)
	lw x15, 56(a1)
	lw x16, 60(a1)
	lw x17, 64(a1)
	lw x18, 68(a1)
	lw x19, 72(a1)
	lw x20, 76(a1)
	lw x21, 80(a1)
	lw x22, 84(a1)
	lw x23, 88(a1)
	lw x24, 92(a1)
	lw x25, 96(a1)
	lw x26, 100(a1)
	lw x27, 104(a1)
	lw x28, 108(a1)
	lw x29, 112(a1)
	lw x30, 116(a1)
	lw x31, 120(a1)

	ret
