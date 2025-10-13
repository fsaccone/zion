.section .text
.global reboot

reboot:
	li a0, 4
	ecall

	ret
