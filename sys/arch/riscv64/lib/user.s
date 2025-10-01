.section .text
.global usermode

usermode:
	csrw sepc, a0

	# Load address at a1.
	mv t0, a1

	# If address is NULL, just set satp to 0 to use physical memory.
	beqz t0, 1f

	# Set MODE to 1010 (Sv57).
	li t1, 0b1010 << 60
	or t0, t0, t1

1:
	csrw satp, t0

	sret
