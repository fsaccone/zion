.section .text
.global disableinterrupts
.global enableinterrupts
.global interruptargs
.global interruptsenabled
.global interrupttype

disableinterrupts:
	csrr t0,      sstatus
	li   t1,      1 << 1
	not  t1,      t1
	and  t0,      t0, t1
	csrw sstatus, t0

	ret

enableinterrupts:
	csrr t0,      sstatus
	li   t1,      1 << 1
	or   t0,      t0, t1
	csrw sstatus, t0

	ret

interruptargs:
	la t0, args
	li t1, 0

	sd a0, (0 * 8)(t0)
	sd a1, (1 * 8)(t0)
	sd a2, (2 * 8)(t0)
	sd a3, (3 * 8)(t0)
	sd a4, (4 * 8)(t0)
	sd t1, (5 * 8)(t0)

	mv a0, t0

	ret

interruptsenabled:
	csrr a0, sstatus
	li   t0, 1 << 1
	and  a0, a0, t0
	srli a0, a0, 1

	ret

interrupttype:
	# code:
	# 0x00 - Unknown
	# 0x01 - Exception
	# 0x02 - Hardware
	# 0x03 - Software
	# 0x04 - Timer
	# 0x05 - Syscall

	csrr t0, scause

	# Interrupt bit (I)
	li t1, 1 << 63

	# If I != 0, code is not an exception nor a syscall
	and  t2, t0, t1
	bnez t2, 1f

	# Exception or Syscall
	# 8 -> Syscall
	# * -> Exception

	# If 8 jump to 2f
	li  t2, 8
	beq t0, t2, 2f

	# Exception (not 8)
	li a0, 0x01
	ret

2:
	# Syscall (8)
	li a0, 0x05
	ret

1:
	# Set I to 0
	not t2, t1
	and t0, t0, t2

	# Interrupt
	# 1 || 3  -> Software
	# 5 || 7  -> Timer
	# 9 || 11 -> Hardware

	# If 1 jump to 2f
	li  t2, 1
	beq t0, t2, 2f

	# If 3 jump to 2f
	li  t2, 3
	beq t0, t2, 2f

	# If 5 jump to 3f
	li  t2, 5
	beq t0, t2, 3f

	# If 7 jump to 3f
	li  t2, 7
	beq t0, t2, 3f

	# If 9 jump to 4f
	li  t2, 9
	beq t0, t2, 4f

	# If 11 jump to 4f
	li  t2, 11
	beq t0, t2, 4f

	# Default (0x00 - Unknown)
	li a0, 0x00
	ret

2:
	# Software
	li a0, 0x03
	ret

3:
	# Timer
	li a0, 0x04
	ret

4:
	# Hardware
	li a0, 0x04
	ret

.section .data

# (5 interrupt arguments + 1 NULL) * 8 bytes
args: .space ((5 + 1) * 8)
