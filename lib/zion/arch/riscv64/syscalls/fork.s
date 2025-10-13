.section .text
.global fork

fork:
	li a0, 0
	ecall

	ret
