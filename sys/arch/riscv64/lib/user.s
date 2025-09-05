.section .text
.global usermode

usermode:
	csrw sepc, a0

	sret
