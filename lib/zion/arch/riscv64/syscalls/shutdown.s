.section .text
.global shutdown

shutdown:
	li a0, 1
	ecall

	ret
