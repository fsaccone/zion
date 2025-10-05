.section .text
.global setuserpc
.global setuserptree
.global usermode

setuserpc:
	la t0, userpc
	sd a0, 0(t0)

	ret

setuserptree:
	la t0, userptree
	sd a0, 0(t0)

	ret

usermode:
	# Set program counter.
	la t0, userpc
	ld t0, 0(t0)
	csrw sepc, t0

	# Load page tree address.
	la t0, userptree
	ld t0, 0(t0)

	# Set MODE to 1010 (Sv57).
	li t1, 0b1010 << 60
	or t0, t0, t1

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

	sret

.section .data
userpc:    .dword 0
userptree: .dword 0
