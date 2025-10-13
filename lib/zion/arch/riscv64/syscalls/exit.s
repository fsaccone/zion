.section .text
.global exit

exit:
	li a0, 0
	ecall

	# Unreachable.
	ret
