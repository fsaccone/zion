.section .text
.global inittimer

inittimer:
	# Set sie.STIE to 1
	csrr t0,  sie
	li   t1,  (1 << 5)
	or   t0,  t0, t1
	csrw sie, t0

	# First timer interrupt
	csrr t0,       time
	li   t1,       1000000
	add  t0,       t0, t1
	csrw stimecmp, t0

	ret
