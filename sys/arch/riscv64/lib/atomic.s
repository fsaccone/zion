.section .text
.global atomicswap

atomicswap:
	amoswap.w.aq a1, a1, 0(a0)
	mv           a0, a1

	ret
