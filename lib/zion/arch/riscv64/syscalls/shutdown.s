.section .text
.global shutdown

shutdown:
	li a0, 0
	ecall

	ret
