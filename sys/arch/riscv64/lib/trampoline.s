.section .text
.global inittrapframe
.global trampoline
.global trampolinebase
.global trampolineret
.global trampolineretbase

# Each trap frame has this structure:
#   [0,      22 * 8 + 7] = Registers.
#   [23 * 8, 23 * 8 + 7] = Kernel satp.
#   [24 * 8, 24 * 8 + 7] = Interrupt handler address.
#   [25 * 8, 25 * 8 + 7] = Kernel stack pointer.
#   [26 * 8, 26 * 8 + 7] = Kernel thread pointer.

inittrapframe:
	# Save kernel satp.
	srli a1, a1, 12
	li   t0, 10 << 60
	or   a1, a1, t0
	sd   a1, (23 * 8)(a0)

	# Set sscratch to given satp.
	csrw sscratch, a1

	# Save interrupt handler address.
	sd a2, (24 * 8)(a0)

	# Set sepc to given pc.
	csrw sepc, a3

	# Set kernel stack pointer.
	sd sp, (25 * 8)(a0)

	# Set kernel thread pointer.
	sd tp, (26 * 8)(a0)

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
	sd t2,  (18 * 8)(t0)
	sd t3,  (19 * 8)(t0)
	sd t4,  (20 * 8)(t0)
	sd t5,  (21 * 8)(t0)
	sd t6,  (22 * 8)(t0)

	# Load rest of user trap frame.
	ld t1, (23 * 8)(t0)
	ld t2, (24 * 8)(t0)
	ld sp, (25 * 8)(t0)
	ld tp, (26 * 8)(t0)

	# Switch to kernel satp and save the old satp to t1.
	csrrw t1, satp, t1

	# Set stvec to interrupt handler.
	csrw stvec, t2

	# Save the old satp to sscratch.
	csrw sscratch, t1

	# Call interrupt handler.
	jalr t2

	# Set s0 to 1 to mark the coming from trampoline, and not
	# trampolineret.
	li s0, 1

	# Skip trampolineret specific code.
	j 1f

	# Continues here.
trampolineret:
	# Set s0 to 0 to mark the coming from trampolineret, and not
	# trampoline.
	li s0, 0

1:
	# Set t6, the last register to be loaded, to the trap frame address.
	li t6, 0x1000

	# Load the old satp and switch to it, saving the kernel satp to t1.
	csrr  t0, sscratch
	csrrw t1, satp, t0

	# Set stvec to trampoline.
	csrwi stvec, 0x0

	# Save the kernel satp to the trap frame.
	sd t1, (23 * 8)(t6)

	# Only save stack pointer and thread pointer to trap frame if we come
	# from trampoline, not trampolineret. If we did come from
	# trampolineret, these components of the trap frames were presumably
	# already set by a call to inittrapframe, also the jump to
	# trampolineret presumably happened through a switchctx call, making
	# the values of sp and tp wrong.
	beqz s0, 1f
	sd   sp, (25 * 8)(t6)
	sd   tp, (26 * 8)(t6)

1:
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
	ld t2,  (18 * 8)(t6)
	ld t3,  (19 * 8)(t6)
	ld t4,  (20 * 8)(t6)
	ld t5,  (21 * 8)(t6)
	ld t6,  (22 * 8)(t6)

	# Return
	sret
.align 12

trampolinebase:
	la a0, trampoline

	ret

trampolineretbase:
	la t0, trampoline
	la t1, trampolineret

	# Since the trampoline it at address 0x0, just get the difference
	# between the phyisical addresses of trampoline and trampolineret to
	# get the virtual address of trampolineret.
	sub a0, t1, t0

	ret
