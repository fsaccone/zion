.section .text
.global usermode
.global usermodebase

usermode:
	# Set program counter.
	mv a0, tp
	call getuserpc
	csrw sepc, a0

	# Enable virtual memory.
	mv a0, tp
	call getuserpc
	li t0, 0b1010 << 60
	or a0, a0, t0
	csrw satp, a0

	# Set stack pointer.
	mv a0, tp
	call getusersp
	mv sp, a0

	# Set sstatus.SPIE to 1 to enable interrupts in user mode.
	csrr t0, sstatus
	li   t1, 1 << 5
	or   t0, t0, t1
	csrw sstatus, t0

	# Set sip.SSIP, sip.STIP and sip.SEIP to 1 to enable all types of
	# interrupts in user mode.
	li   t0, 0
	li   t1,  1 << 1
	or   t0,  t0, t1
	li   t1,  1 << 5
	or   t0,  t0, t1
	li   t1,  1 << 9
	or   t0,  t0, t1
	csrw sip, t0

	# Set sstatus.SPP to 0 to make sret switch to user mode.
	csrr t0, sstatus
	li   t1, 1 << 8
	not  t1, t1
	and  t0, t0, t1
	csrw sstatus, t0

	sret

usermodebase:
	la a0, usermode

	ret
