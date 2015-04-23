bits 64

global main

extern printf

section .text
main:
    push        rbp
    mov         rbp, rsp

    fld         qword [a]
    fmul        st0
    
    fld         qword [b]
    fmul        st0, st0
    fadd        st0, st1
    fsqrt
    fstp        qword [c]

    movq        xmm0, qword [c]
    mov         rdi, fmt_str
    call        printf

    mov         rax, 0
    pop         rbp

    ret

section .data
    fmt_str:    db "%lf", 0x0A, 0x00
    a:          dq 3.0
    b:          dq 4.0
    c:          dq 0.0