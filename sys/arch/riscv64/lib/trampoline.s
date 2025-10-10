.section .text
.global setuptrampolineret
.global trampoline
.global trampolinebase
.global trampolineret
.global trampolineretbase

# Each user trap frame has this structure:
#   [0,      22 * 8 + 7] = Registers.
#   [23 * 8, 23 * 8 + 7] = Kernel satp.
#   [24 * 8, 24 * 8 + 7] = Kernel stack pointer.
#   [25 * 8, 25 * 8 + 7] = Kernel thread pointer.
#   [26 * 8, 26 * 8 + 7] = Interrupt handler address.
#
# The kernel trap frame has this structure:
#   [0, 7]     = Caller satp.
#   [8, 8 + 7] = Interrupt handler address.

setuptrampolineret:
	# Set t0 to the kernel trap frame address.
	li t0, 0x1000

	# Save satp.
	srli a0, a0, 12
	li   t1, 10 << 60
	or   a0, a0, t1
	sd   a0, 0(t0)

	# Save interrupt handler address.
	sd a1, 8(t0)

	# Set sepc to given pc.
	csrw sepc, a2

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

	# Set stvec to trampoline.
	csrwi stvec, 0x0

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

	# Load user trap frame.
	ld t1, (23 * 8)(t0)
	ld sp, (24 * 8)(t0)
	ld tp, (25 * 8)(t0)
	ld t2, (26 * 8)(t0)

	# Switch to kernel satp and save the old satp to t1.
	csrrw t1, satp, t1

	# Save the old satp to the now available kernel trap frame.
	sd t1, 0(t0)

	# Call interrupt handler.
	jalr t2

	# Continues here.
trampolineret:
	# Set t6, the last register to be loaded, to the trap frame address.
	li t6, 0x1000

	# Load the interrupt handler address from the kernel trap frame to t2.
	ld t2, 8(t6)

	# Load the old satp and switch to it, saving the kernel satp to t1.
	ld    t0,   0(t6)
	csrrw t1, satp, t0

	# Save the user trap frame from the current context.
	sd t1, (23 * 8)(t6)
	sd sp, (24 * 8)(t6)
	sd tp, (25 * 8)(t6)
	sd t2, (26 * 8)(t6)

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
