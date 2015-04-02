bits 64

global main

extern printf

section .text
main:
    push        rbp
    mov         rbp, rsp

    movsd       xmm0, qword [dbl]
    mov         rdi, fmt_str
    call        printf

    mov         rax, 0
    pop         rbp

    ret

section .data
    fmt_str:    db "Hello from %lf", 0x00
    dbl:        dq 3.14159265358