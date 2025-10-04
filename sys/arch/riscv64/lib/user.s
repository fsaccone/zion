.section .text
.global setuserptree
.global usermode

setuserptree:
	la t0, userptree
	ld a0, 0(t0)

	ret

usermode:
	csrwi sepc, 0x0

	# Load page tree address.
	la t0, userptree

	# Set MODE to 1010 (Sv57).
	li t1, 0b1010 << 60
	or t0, t0, t1

	csrw satp, t0

	sret

.section .data
userptree: .dword 0
