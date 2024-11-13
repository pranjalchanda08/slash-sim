    .align 2

    .section .text
    .globl _start

_start:
    li a0, 0xF0F0          
    li a1, 0x0F            

    ANDI a2, a0, 0x0F      

    li a3, 0x03            
    SLLI a4, a2, 2         

    SRLI a5, a2, 3         

    li a7, 10              

    .section .data
