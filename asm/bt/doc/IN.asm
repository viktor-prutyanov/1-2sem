bits 64
;IN: scan number from stdin to rax
    push    rbp
    mov     rbp, rsp    

    sub     rbp, 32

    mov     rdx, 32             ;message length
    mov     rsi, rbp            ;message to read
    mov     rdi, 0              ;file descriptor (stdout)
    mov     rax, 0              ;system call number (sys_read)
    syscall

    mov     r9, rbp
    dec     r9
    mov     rcx, rbp
    add     rcx, rax
    sub     rcx, 2
    mov     r10, 1
    mov     rbx, 10
    xor     rax, rax
    xor     r8, r8

next:
    xor     rax, rax
    mov     al, byte [rcx]
    sub     al, '0'
    mul     r10
    add     r8, rax

    mov     rax, r10
    mul     rbx
    mov     r10, rax

    dec     rcx
    cmp     rcx, r9
    jne     next

    pop     rbp

    mov     rax, r8