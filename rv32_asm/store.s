    .align 4
    .section .text
    .globl _start

_start:
    li a0, 0x13
    li a1, 0x23
    add a2, a0, a1
    sb a0, 0(a4)
    lbu t0, 0(a4)
    sh a1, 2(a4)
    lhu t1, 2(a4)
    sw a2, 4(a4)
    lw t2, 4(a4)

.section .data
a4: 
    .space 16
