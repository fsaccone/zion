.section .text
.global start

start:
	call allocstack
	mv   sp, a0

	call raminit

	# Do shutdown().
	li a0, 3
	ecall

1:
	j 1b

# Allocates stack and returns its end address.
allocstack:
	# Do growmem(8192).
	li a0, 2
	li a1, 8192
	ecall

	# Since stack is 2 pages, add (2 * PAGE_SIZE - 1) to the first address
	# to get the end address.
	li  t0, 8192 - 1
	add a0, a0, t0

	ret
