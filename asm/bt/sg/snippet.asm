bits 64
;push    qword 'V'
mov     rax, 0x01           ; sys_write
mov     rdi, 1              ; file descr. 1 (stdout)
mov     rsi, rsp            ; pointer to the data
mov     rdx, 1              ; amount of data
syscall                     ; call to the kernel
;pop     rax