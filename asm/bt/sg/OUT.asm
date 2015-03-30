bits 64                         
;OUT: number will be pushed and print to stdout
    pop     rax 

    push    rbp
    mov     rbp, rsp
    
    dec     rbp
    mov     byte [rbp], 0x0A

    xor     rdx, rdx            
    mov     rbx, 10
    mov     rcx, 1

next_div:
    inc     rcx
    xor     rdx, rdx
    div     rbx
    add     rdx, 0x30
    dec     rbp
    mov     byte [rbp], dl
    cmp     rax, 0
    jne     next_div

    mov     rax, 0x01           ; sys_write
    mov     rdi, 1              ; file descriptor 1 (stdout)
    mov     rsi, rbp            ; pointer to the data
    mov     rdx, rcx            ; amount of data
    syscall                     ; call to the kernel

    pop     rbp