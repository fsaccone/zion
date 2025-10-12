.section .text
.global inittrapframe
.global preparetrapframe
.global setreturn
.global trampoline
.global trampolinebase
.global usermode
.global usermodebase

# The following are the indexes of a trap frame as an array of u64:
#   0-28 = (T)   Registers.
#   29   = (T)   Kernel satp.
#   30   = (I)   Kernel interrupt entry point.
#   31   = (I)   User interrupt entry point.
#   32   = (I)   User interrupt return address.
#   33   = (I,T) User sepc.
#   34   = (P)   Kernel stack pointer.
#   35   = (P)   Kernel thread pointer.
#   36   = (T)*  Return value.
# The flags represent where each element is modified:
#   - I: Modified in inittrapframe.
#   - P: Modified in preparetrapframe.
#   - T: Modified in trampoline (* Return value may always be modified).

inittrapframe:
	# Save kernel interrupt entry point.
	la t0, kernelinterrupt
	sd t0, (30 * 8)(a0)

	# Save user interrupt entry point.
	la t0, userinterrupt
	sd t0, (31 * 8)(a0)

	# Save user interrupt return address as the difference between the
	# physical addresses of trampolineret and trampoline plus the virtual
	# address of trampoline, which is 0x0.
	la  t0, trampoline
	la  t1, trampolineret
	sub t0, t1, t0
	sd  t0, (32 * 8)(a0)

	# Set user sepc to initial pc.
	sd a1, (33 * 8)(a0)

	# Set stack pointer of user context to initial sp.
	sd a2, (3 * 8)(a0)

	ret

preparetrapframe:
	# Set kernel stack pointer.
	sd sp, (34 * 8)(a0)

	# Set kernel thread pointer.
	sd tp, (35 * 8)(a0)

	# Set sscratch to given satp.
	srli a1, a1, 12
	li   t0, 10 << 60
	or   a1, a1, t0
	csrw sscratch, a1

	ret

setreturn:
	sd a1, (36 * 8)(a0)

	ret

.align 12
trampoline:
	# Swap t0 and sscratch, set t0 to the trap frame address and save t1.
	csrrw t0, sscratch, t0
	li    t0, 0x1000
	sd    t1, (0 * 8)(t0)

	# Save the original t0.
	csrr t1, sscratch
	sd   t1, (1 * 8)(t0)

	# Save all the other registers.
	sd ra,  (2  * 8)(t0)
	sd sp,  (3  * 8)(t0)
	sd gp,  (4  * 8)(t0)
	sd tp,  (5  * 8)(t0)
	sd s0,  (6  * 8)(t0)
	sd s1,  (7  * 8)(t0)
	sd s2,  (8  * 8)(t0)
	sd s3,  (9  * 8)(t0)
	sd s4,  (10 * 8)(t0)
	sd s5,  (11 * 8)(t0)
	sd s6,  (12 * 8)(t0)
	sd s7,  (13 * 8)(t0)
	sd s8,  (14 * 8)(t0)
	sd s9,  (15 * 8)(t0)
	sd s10, (16 * 8)(t0)
	sd s11, (17 * 8)(t0)
	sd a1,  (18 * 8)(t0)
	sd a2,  (19 * 8)(t0)
	sd a3,  (20 * 8)(t0)
	sd a4,  (21 * 8)(t0)
	sd a5,  (22 * 8)(t0)
	sd a6,  (23 * 8)(t0)
	sd t2,  (24 * 8)(t0)
	sd t3,  (25 * 8)(t0)
	sd t4,  (26 * 8)(t0)
	sd t5,  (27 * 8)(t0)
	sd t6,  (28 * 8)(t0)

	# Load rest of user trap frame.
	ld t1, (29 * 8)(t0)
	ld t2, (30 * 8)(t0)
	ld t3, (31 * 8)(t0)
	ld ra, (32 * 8)(t0)
	ld sp, (34 * 8)(t0)
	ld tp, (35 * 8)(t0)

	# Set return value to current a0, so that it does not change if not
	# done explicitly.
	sd a0, (36 * 8)(t0)

	# Switch to kernel satp and save the old satp to t1.
	csrrw t1, satp, t1

	# Set stvec to kernel interrupt entry point.
	csrw stvec, t2

	# Save the old satp to sscratch.
	csrw sscratch, t1

	# Call user interrupt entry point: ra is already set to trampolineret.
	jalr t3

trampolineret:
	# Set s0 to 1 to mark the coming from trampoline, and not usermode.
	li s0, 1

	# Skip usermode specific code.
	j 1f

usermode:
	# Set s0 to 0 to mark the coming from usermode, and not trampoline.
	li s0, 0

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

	# Load sepc.
	ld   t0,   (33 * 8)(a0)
	csrw sepc, t0

1:
	# Set t6, the last register to be loaded, to the trap frame address.
	li t6, 0x1000

	# Load the old satp and switch to it, saving the kernel satp to t1.
	csrr  t0, sscratch
	csrrw t1, satp, t0

	# Set stvec to trampoline.
	csrwi stvec, 0x0

	# Save the kernel satp to the trap frame.
	sd t1, (29 * 8)(t6)

	# Only save stack pointer and thread pointer to trap frame if we come
	# from trampoline, not usermode. If we did come from usermode, these
	# components of the trap frames were presumably already set by a call
	# to inittrapframe, also the jump to usermode presumably happened
	# through a switchctx call, making the values of sp and tp wrong.
	beqz s0, 1f
	sd   sp, (34 * 8)(t6)
	sd   tp, (35 * 8)(t6)

1:
	# Load return value from stack frame to a0.
	ld a0, (36 * 8)(t6)

	# Load the saved registers from the now available user trap frame.
	ld t1,  (0  * 8)(t6)
	ld t0,  (1  * 8)(t6)
	ld ra,  (2  * 8)(t6)
	ld sp,  (3  * 8)(t6)
	ld gp,  (4  * 8)(t6)
	ld tp,  (5  * 8)(t6)
	ld s0,  (6  * 8)(t6)
	ld s1,  (7  * 8)(t6)
	ld s2,  (8  * 8)(t6)
	ld s3,  (9  * 8)(t6)
	ld s4,  (10 * 8)(t6)
	ld s5,  (11 * 8)(t6)
	ld s6,  (12 * 8)(t6)
	ld s7,  (13 * 8)(t6)
	ld s8,  (14 * 8)(t6)
	ld s9,  (15 * 8)(t6)
	ld s10, (16 * 8)(t6)
	ld s11, (17 * 8)(t6)
	ld a1,  (18 * 8)(t6)
	ld a2,  (19 * 8)(t6)
	ld a3,  (20 * 8)(t6)
	ld a4,  (21 * 8)(t6)
	ld a5,  (22 * 8)(t6)
	ld a6,  (23 * 8)(t6)
	ld t2,  (24 * 8)(t6)
	ld t3,  (25 * 8)(t6)
	ld t4,  (26 * 8)(t6)
	ld t5,  (27 * 8)(t6)
	ld t6,  (28 * 8)(t6)

	# Return
	sret
.align 12

trampolinebase:
	la a0, trampoline

	ret

usermodebase:
	la t0, trampoline
	la t1, usermode

	# Since the trampoline it at address 0x0, just get the difference
	# between the phyisical addresses of trampoline and usermode to get the
	# virtual address of usermode.
	sub a0, t1, t0

	ret
