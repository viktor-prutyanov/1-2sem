bits 64                         
;OUT: number will be pushed and print to stdout
    mov     rax, 0x01           ; sys_write
    mov     rdi, 1              ; file descriptor 1 (stdout)
    mov     rsi, rbp            ; pointer to the data
    mov     rdx, rcx            ; amount of data
    syscall                     ; call to the kernel