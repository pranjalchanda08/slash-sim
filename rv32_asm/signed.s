.align 2

.section .text
.globl _start
.globl stack_top

_start:
	la sp, stack_top
	li t0, -0x0000000F
	li t1, -0x00000002

	addi t2, t0, -1
	slti t3, t0, -16
	sltiu t4, t0, 0xFFFFFFF0
	xori t5, t0, -0x0000000F
	ori t6, t0, -0x00000001

.section .data
