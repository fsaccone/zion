.section .text
.global memswitch

memswitch:
	# Set satp.
	srli a0,   a0, 12
	li   t0,   10 << 60
	or   a0,   a0, t0
	csrw satp, a0

	ret
