.align 2

.section .text
.globl _start
.globl stack_top

_start:
	la sp, stack_top

	li t0, 0x00000013
	li t1, 2

	and t2, t0, t1

.section .data
