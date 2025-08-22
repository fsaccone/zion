.section .entry.text
.globl _entry

_entry:
	la sp, _stack_end

	j bmain
