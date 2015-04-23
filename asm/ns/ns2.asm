bits 64

section     .text
global      _start

_start:
    mov     rax, 0
    mov     rsi, buffer
    mov     rdx, 0xFF
    mov     rdi, 0
    syscall
    sub     rax, 1

    mov     rsi, rax
    mov     rdi, 8 ; From system base
    call    convert_from

    mov     rsi, rax
    mov     rdi, 16 ; To system base
    call    convert_to

    mov     rsi, buffer
    add     rsi, 0xFF
    sub     rsi, rax
    mov     rdi, 1   
    mov     rdx, rax 
    add     rdx, 1
    mov     rax, 0x01
    syscall          

    mov     rax, 0x3c                   ;system call number (sys_exit)
    mov     rdi, 0                      ;exitcode   
    syscall

; In: System -> RDI, Amount of symbols -> RSI
; Out: Number -> RAX
convert_from:
    xor     r8, r8
    mov     rcx, rsi
    mov     rbx, 1
next_digit:
    xor     rax, rax
    mov     al, byte [buffer + rcx - 1]
    cmp     al, '9'
    jbe     digit1
    sub     al, 'a' - 10 - '0'
digit1:
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

; In: System -> RDI, Number -> RSI
; Out: Amount of symbols -> RAX
convert_to:
    mov     rax, rsi
    mov     r8, buffer
    add     r8, 0xFF
    mov     byte [r8], 0x0A
    xor     rdx, rdx   
    mov     rbx, rdi
    mov     rcx, 1
next_div:
    inc     rcx
    xor     rdx, rdx
    div     rbx
    cmp     dl, 10
    jb      digit2
    add     dl, 'a' - 10 - '0'
digit2:
    add     dl, '0'
    dec     r8
    mov     byte [r8], dl
    or      rax, rax
    jne     next_div

    mov     rax, rcx        

    ret 

section     .data
    buffer  times 0xFF db 0x00