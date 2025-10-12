.section .text
.global memswitch
.global pmemswitch

memswitch:
	sfence.vma zero, zero

	srli a0,   a0, 12
	li   t0,   10 << 60
	or   a0,   a0, t0
	csrw satp, a0

	sfence.vma zero, zero

	ret

pmemswitch:
	sfence.vma zero, zero

	csrwi satp, 0

	sfence.vma zero, zero

	ret
