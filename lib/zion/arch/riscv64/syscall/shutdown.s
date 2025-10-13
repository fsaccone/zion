.section .text
.global shutdown

shutdown:
	li a0, 3
	ecall

	ret
