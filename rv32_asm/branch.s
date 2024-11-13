    .align 2

    .section .text
    .globl _start

_start:
    li a0, 5                
    li a1, 5                

    beq a0, a1, branch_here 

    li a2, 0               
    j end                    

branch_here:
    li a2, 1                

end:

.section .data
