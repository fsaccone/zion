.section .text
.global reboot

reboot:
	li a0, 3
	ecall

	ret
