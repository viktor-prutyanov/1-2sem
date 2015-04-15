%macro Exit 1
    mov         rax, 0x3c                   ;system call number (sys_exit)
    mov         rdi, %1                     ;exitcode   
    syscall
%endmacro

bits 64

%define BUF_LEN 0xFF

section     .text
global      _start
_start:
    mov     rax, 0
    mov     rsi, buffer
    mov     rdx, BUF_LEN
    mov     rdi, 0
    syscall
    sub     rax, 1

    mov     rsi, rax
    mov     rdi, 10
    call    convert_from

    mov     rsi, rax
    mov     rdi, 2
    call    convert_to

    mov     rdx, rax 
    mov     rax, 0x01
    mov     rdi, 1   
    mov     rsi, rbx 
    syscall          

    Exit    0

convert_from:
    xor     r8, r8
    mov     rcx, rsi
    mov     rbx, 1
next_digit:
    xor     rax, rax
    mov     al, byte [buffer + rcx - 1]
    sub     al, '0'
    mul     rbx
    add     r8, rax
    mov     rax, rbx
    mul     rdi
    mov     rbx, rax
    dec     rcx
    jnz     next_digit
    mov     rax, r8
    ret



convert_to:
    push    rbp
    mov     rbp, rsp
    
    mov     rax, rsi

    dec     rbp
    mov     byte [rbp], 0x0A

    xor     rdx, rdx            
    mov     rbx, rdi
    mov     rcx, 1
next_div:
    inc     rcx
    xor     rdx, rdx
    div     rbx
    add     rdx, '0'
    dec     rbp
    mov     byte [rbp], dl
    or      rax, rax
    jne     next_div

    mov     rbx, rbp
    mov     rax, rcx        

    pop     rbp
    ret 

section     .data
    buffer  times BUF_LEN db 0x00