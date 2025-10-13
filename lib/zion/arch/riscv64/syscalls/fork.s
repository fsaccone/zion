.section .text
.global fork

fork:
	li a0, 1
	ecall

	ret
