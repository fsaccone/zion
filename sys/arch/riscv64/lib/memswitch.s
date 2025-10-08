.section .text
.global memswitch

memswitch:
	# Load max address as stack pointer.
	li  sp, 0
	not sp, sp

	# Set satp.
	csrw satp, a0

	ret
