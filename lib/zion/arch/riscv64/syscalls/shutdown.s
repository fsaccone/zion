.section .text
.global shutdown

shutdown:
	li a0, 2
	ecall

	ret
