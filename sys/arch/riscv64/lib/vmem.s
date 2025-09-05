.section .text
.global memswitch

memswitch:
	# If a0 == NULL, set satp to 0
	li t0, 0
	beqz a0, 1f

	# Remove the trailing zeros from the address, aligned to 4096
	srli a0, a0, 12

	# Put the ASID at the right position
	slli a1, a1, 44

	# Merge a0 and a1 in t0
	or t0, t0, a0
	or t0, t0, a1

	# Set MODE to 1010 (Sv57)
	li t1, 0b1010 << 60
	or t0, t0, t1

1:
	csrw satp, t0

	ret
