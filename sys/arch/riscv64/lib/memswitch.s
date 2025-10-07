.section .text
.global memswitch

memswitch:
	csrw satp, a0

	ret
