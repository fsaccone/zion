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
	csrw sepc, t0

	# Load page tree address.
	la t0, userptree

	# Set MODE to 1010 (Sv57).
	li t1, 0b1010 << 60
	or t0, t0, t1

	csrw satp, t0

	sret

.section .data
userpc:    .dword 0
userptree: .dword 0
