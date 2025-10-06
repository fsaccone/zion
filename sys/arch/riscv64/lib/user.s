.section .text
.global userinterruptbase
.global usermode
.global usermodebase

.align 12
userinterrupt:
	csrwi satp, 0

	# Reset kernel interrupt handler.
	la   t0,    interrupt
	csrw stvec, t0

	j interrupt
.align 12

userinterruptbase:
	la a0, userinterrupt

	ret

usermode:
	# Set program counter.
	mv a0, tp
	call getuserpc
	csrw sepc, a0

	# Save page tree address (shifted right of 12 bits) to satp, without
	# enabling virtual memory.
	mv   a0,   tp
	call getuserpc
	srli t0,   a0, 12
	csrw satp, t0

	# Set sstatus.SPIE to 1 to enable interrupts in user mode.
	csrr t0, sstatus
	li   t1, 1 << 5
	or   t0, t0, t1
	csrw sstatus, t0

	# Set sstatus.SPP to 0 to make sret switch to user mode.
	csrr t0, sstatus
	li   t1, 1 << 8
	not  t1, t1
	and  t0, t0, t1
	csrw sstatus, t0

	# Set stack pointer.
	mv a0, tp
	call getusersp
	mv sp, a0

	# Enable virtual memory.
	csrr t0,   satp
	li   t1,   0b1010 << 60
	or   t0,   t0, t1
	csrw satp, t0

	# Set stvec to 0x0.
	csrwi stvec, 0x0

	sret

usermodebase:
	la a0, usermode

	ret
