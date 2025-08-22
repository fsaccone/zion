.section .entry.text
.globl _start

_start:
	la sp, _stack_end

	j bmain
