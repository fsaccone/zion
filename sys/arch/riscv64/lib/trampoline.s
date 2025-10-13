.section .text
.global copytrapframe
.global inittrapframe
.global preparetrapframe
.global setreturn
.global trampoline
.global trampolinebase
.global usermode
.global usermodebase

# The following are the indexes of a trap frame as an array of u64:
#   - Data saved by inittrapframe:
#     0 = Kernel interrupt entry point.
#     1 = User interrupt entry point.
#     2 = User interrupt return address.
#     3 = Trampoline virtual address.
#
#   - Data saved by preparetrapframe and modified by trampolineret:
#     10 = Kernel stack pointer.
#     11 = Kernel thread pointer.
#
#   - Data saved by trampoline:
#     20    = User sepc.
#     21    = Kernel satp.
#     22    = Return value.
#     23-51 = Registers.

copytrapframe:
	# Copy user sepc.
	ld t0, (20 * 8)(a1)
	sd t0, (20 * 8)(a0)

	# Copy registers.
	ld t0, (23 * 8)(a1)
	sd t0, (23 * 8)(a0)
	ld t0, (24 * 8)(a1)
	sd t0, (24 * 8)(a0)
	ld t0, (25 * 8)(a1)
	sd t0, (25 * 8)(a0)
	ld t0, (26 * 8)(a1)
	sd t0, (26 * 8)(a0)
	ld t0, (27 * 8)(a1)
	sd t0, (27 * 8)(a0)
	ld t0, (28 * 8)(a1)
	sd t0, (28 * 8)(a0)
	ld t0, (29 * 8)(a1)
	sd t0, (29 * 8)(a0)
	ld t0, (30 * 8)(a1)
	sd t0, (30 * 8)(a0)
	ld t0, (31 * 8)(a1)
	sd t0, (31 * 8)(a0)
	ld t0, (32 * 8)(a1)
	sd t0, (32 * 8)(a0)
	ld t0, (33 * 8)(a1)
	sd t0, (33 * 8)(a0)
	ld t0, (34 * 8)(a1)
	sd t0, (34 * 8)(a0)
	ld t0, (35 * 8)(a1)
	sd t0, (35 * 8)(a0)
	ld t0, (36 * 8)(a1)
	sd t0, (36 * 8)(a0)
	ld t0, (37 * 8)(a1)
	sd t0, (37 * 8)(a0)
	ld t0, (38 * 8)(a1)
	sd t0, (38 * 8)(a0)
	ld t0, (39 * 8)(a1)
	sd t0, (39 * 8)(a0)
	ld t0, (40 * 8)(a1)
	sd t0, (40 * 8)(a0)
	ld t0, (41 * 8)(a1)
	sd t0, (41 * 8)(a0)
	ld t0, (42 * 8)(a1)
	sd t0, (42 * 8)(a0)
	ld t0, (43 * 8)(a1)
	sd t0, (43 * 8)(a0)
	ld t0, (44 * 8)(a1)
	sd t0, (44 * 8)(a0)
	ld t0, (45 * 8)(a1)
	sd t0, (45 * 8)(a0)
	ld t0, (46 * 8)(a1)
	sd t0, (46 * 8)(a0)
	ld t0, (47 * 8)(a1)
	sd t0, (47 * 8)(a0)
	ld t0, (48 * 8)(a1)
	sd t0, (48 * 8)(a0)
	ld t0, (49 * 8)(a1)
	sd t0, (49 * 8)(a0)
	ld t0, (50 * 8)(a1)
	sd t0, (50 * 8)(a0)
	ld t0, (51 * 8)(a1)
	sd t0, (51 * 8)(a0)

	ret

inittrapframe:
	# Save kernel interrupt entry point.
	la t0, kernelinterrupt
	sd t0, (0 * 8)(a0)

	# Save user interrupt entry point.
	la t0, userinterrupt
	sd t0, (1 * 8)(a0)

	# Save user interrupt return address as the virtual address of
	# trampolineret: that is the difference between the physical addresses
	# of trampolineret and trampoline plus the virtual address of
	# trampoline.
	la  t0, trampoline
	la  t1, trampolineret
	sub t0, t1, t0
	add t0, t0, a2
	sd  t0, (2 * 8)(a0)

	# Set user sepc to initial pc.
	sd a1, (20 * 8)(a0)

	# Save trampoline virtual address.
	sd a2, (3 * 8)(a0)

	ret

preparetrapframe:
	# Set kernel stack pointer.
	sd sp, (10 * 8)(a0)

	# Set kernel thread pointer.
	sd tp, (11 * 8)(a0)

	# Set sscratch to given satp.
	srli a1, a1, 12
	li   t0, 10 << 60
	or   a1, a1, t0
	csrw sscratch, a1

	ret

setreturn:
	sd a1, (22 * 8)(a0)

	ret

.align 12
trampoline:
	# Swap t0 and sscratch, set t0 to the trap frame address and save t1.
	csrrw t0, sscratch, t0
	li    t0, 0x1000
	sd    t1, (23 * 8)(t0)

	# Save the original t0.
	csrr t1, sscratch
	sd   t1, (24 * 8)(t0)

	# Save all the other registers.
	sd ra,  (25 * 8)(t0)
	sd sp,  (26 * 8)(t0)
	sd gp,  (27 * 8)(t0)
	sd tp,  (28 * 8)(t0)
	sd s0,  (29 * 8)(t0)
	sd s1,  (30 * 8)(t0)
	sd s2,  (31 * 8)(t0)
	sd s3,  (32 * 8)(t0)
	sd s4,  (33 * 8)(t0)
	sd s5,  (34 * 8)(t0)
	sd s6,  (35 * 8)(t0)
	sd s7,  (36 * 8)(t0)
	sd s8,  (37 * 8)(t0)
	sd s9,  (38 * 8)(t0)
	sd s10, (39 * 8)(t0)
	sd s11, (40 * 8)(t0)
	sd a1,  (41 * 8)(t0)
	sd a2,  (42 * 8)(t0)
	sd a3,  (43 * 8)(t0)
	sd a4,  (44 * 8)(t0)
	sd a5,  (45 * 8)(t0)
	sd a6,  (46 * 8)(t0)
	sd t2,  (47 * 8)(t0)
	sd t3,  (48 * 8)(t0)
	sd t4,  (49 * 8)(t0)
	sd t5,  (50 * 8)(t0)
	sd t6,  (51 * 8)(t0)

	# If the cause of the interrupt was an ecall exception, set sepc to the
	# instruction right after ecall.
	csrr t1, scause
	li   t2, 8
	beq  t1, t2, 1f
	j 2f
1:
	csrr t1,   sepc
	addi t1,   t1, 4
	csrw sepc, t1
2:

	# Save sepc.
	csrr t1, sepc
	sd   t1, (20 * 8)(t0)

	# Load fixed kernel context from trap frame.
	ld t1, (0 * 8)(t0) # Kernel interrupt entry point.
	ld t2, (1 * 8)(t0) # User interrupt entry point.
	ld ra, (2 * 8)(t0) # User interrupt return address.

	# Load kernel context saved from last trampolineret or
	# preparetrapframe.
	ld sp, (10 * 8)(t0) # Kernel stack pointer.
	ld tp, (11 * 8)(t0) # Kernel thread pointer.
	ld t3, (21 * 8)(t0) # Kernel satp.

	# Set return value to current a0, so that it does not change if not
	# done explicitly.
	sd a0, (22 * 8)(t0)

	# Switch to kernel satp and save the old satp to t3.
	sfence.vma zero, zero
	csrrw      t3,   satp, t3
	sfence.vma zero, zero

	# Set stvec to kernel interrupt entry point.
	csrw stvec, t1

	# Save the old satp to sscratch.
	csrw sscratch, t3

	# Call user interrupt entry point: ra is already set to trampolineret.
	jalr t2

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

1:
	# Set t6, the last register to be loaded, to the trap frame address.
	li t6, 0x1000

	# Load the old satp and switch to it, saving the kernel satp to t1.
	csrr       t0,   sscratch
	sfence.vma zero, zero
	csrrw      t1,   satp, t0
	sfence.vma zero, zero

	# Load sepc.
	ld   t0,   (20 * 8)(t6)
	csrw sepc, t0

	# Set stvec to trampoline virtual address.
	ld   t0,    (3 * 8)(t6)
	csrw stvec, t0

	# Save the kernel satp to the trap frame.
	sd t1, (21 * 8)(t6)

	# Only save stack pointer and thread pointer to trap frame if we come
	# from trampoline, not usermode. If we did come from usermode, these
	# components of the trap frames were presumably already set by a call
	# to inittrapframe. Also, the jump to usermode presumably happened
	# through a switchctx call, making the values of sp and tp wrong.
	beqz s0, 1f
	sd   sp, (10 * 8)(t6)
	sd   tp, (11 * 8)(t6)

1:
	# Load return value from trap frame to a0.
	ld a0, (22 * 8)(t6)

	# Load the saved registers from the now available user trap frame.
	ld t1,  (23 * 8)(t6)
	ld t0,  (24 * 8)(t6)
	ld ra,  (25 * 8)(t6)
	ld sp,  (26 * 8)(t6)
	ld gp,  (27 * 8)(t6)
	ld tp,  (28 * 8)(t6)
	ld s0,  (29 * 8)(t6)
	ld s1,  (30 * 8)(t6)
	ld s2,  (31 * 8)(t6)
	ld s3,  (32 * 8)(t6)
	ld s4,  (33 * 8)(t6)
	ld s5,  (34 * 8)(t6)
	ld s6,  (35 * 8)(t6)
	ld s7,  (36 * 8)(t6)
	ld s8,  (37 * 8)(t6)
	ld s9,  (38 * 8)(t6)
	ld s10, (39 * 8)(t6)
	ld s11, (40 * 8)(t6)
	ld a1,  (41 * 8)(t6)
	ld a2,  (42 * 8)(t6)
	ld a3,  (43 * 8)(t6)
	ld a4,  (44 * 8)(t6)
	ld a5,  (45 * 8)(t6)
	ld a6,  (46 * 8)(t6)
	ld t2,  (47 * 8)(t6)
	ld t3,  (48 * 8)(t6)
	ld t4,  (49 * 8)(t6)
	ld t5,  (50 * 8)(t6)
	ld t6,  (51 * 8)(t6)

	# Return
	sret
.align 12

trampolinebase:
	la a0, trampoline

	ret

usermodebase:
	la t0, trampoline
	la t1, usermode

	# Add the virtual address of trampoline to the difference between the
	# physical addresses of usermode and trampoline to get the virtual
	# address of usermode.
	mv  t2, t1
	sub t2, t2, t0
	add t2, t2, a0
	mv  a0, t2

	ret
