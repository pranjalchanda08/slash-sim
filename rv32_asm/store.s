    .align 4
    .section .text
    .globl _start

_start:
    la a4, _data
    li a0, 0x13
    sb a0, 1(a4)

.section .data
_data: 
    .space 16
