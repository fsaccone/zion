.section .text
.global setupnexttimer

setupnexttimer:
	csrr t0,       time
	li   t1,       1000000
	add  t0,       t0, t1
	csrw stimecmp, t0

	ret
