bits 64

global main

extern printf
extern scanf

section .text
main:
    push        rbp
    mov         rbp, rsp

    mov         rsi, buf
    mov         rdi, fmt   
    call        scanf

    movsd       xmm0, qword [buf]
    ;movsd       xmm1, qword [dbl]
    ;addsd       xmm0, xmm1
    mov         rdi, fmt
    call        printf

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    mov         rax, 0
    pop         rbp

    ret

section .data
    fmt:        db "%lf", 0x00
    buf:        dq 0x00