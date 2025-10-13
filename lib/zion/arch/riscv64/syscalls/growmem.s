.section .text
.global growmem

growmem:
	mv a1, a0
	li a0, 1
	ecall

	ret
