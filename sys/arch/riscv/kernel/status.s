.section .text
.globl setmstatus

setmstatus:
	csrr t0, mstatus

	# Set MPP[0] to 1
	li t1, 1 << 11
	or t0, t0, t1

	# Set MPP[1] to 0 (MPP = 0b01)
	li  t1, 1 << 12
	not t1, t1
	and t0, t0, t1

	# Set MPIE to 0
	li  t1, 1 << 7
	not t1, t1
	and t0, t0, t1

	csrw mstatus, t0

	ret
