.section .text
.global _start

_start:
	# Set sp to growmem(8192) + 8191.
	li  a0, 2
	li  a1, 8192
	ecall
	li  t0, 8191
	add sp, a0, t0

	call main

	# Do exit().
	li a0, 0
	ecall
