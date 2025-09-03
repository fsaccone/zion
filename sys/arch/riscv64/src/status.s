.section .text
.global setmstatus
.global setsstatus

setmstatus:
	csrr t0, mstatus

	# Set SIE to 1
	li t1, 1 << 1
	or t0, t0, t1

	# Set MIE to 1
	li t1, 1 << 3
	or t0, t0, t1

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

setsstatus:
	csrr t0, sstatus

	# Set SUM to 1
	li t1, 1 << 18
	or t0, t0, t1

	# Set MXR to 1
	li t1, 1 << 19
	or t0, t0, t1

	csrw sstatus, t0

	ret
