;   Custom printf realization
;   03.2015
;   GNU GPL v2.0
;   Viktor Prutyanov mailto:vitteran@gmail.com

;Prints string in stdout
;Out: RAX <- amount of symbols written
;Dstr: RDI, RSI, RDX
%macro Print 2
    mov         rax, 0x01                   ;system call number (sys_write) 
    mov         rdi, 0x01                   ;file descriptor (stdout)
    mov         rsi, %1
    mov         rdx, %2
    syscall
%endmacro

;Exit program with code
;Out: None
;Dstr: RAX, RDI
%macro Exit 1
    mov         rax, 0x3c                   ;system call number (sys_exit)
    mov         rdi, %1                     ;exitcode   
    syscall
%endmacro

;Reads char from stdout
;Out: RAX <- amount of symbols read
;Dstr: RDI, RSI, RDX
%macro Read 2
    mov         rax, 0x00                   ;system call number (sys_read)
    mov         rdi, 0x00                   ;file descriptor (stdin)
    mov         rsi, %1  
    mov         rdx, %2
    syscall
%endmacro

bits 64

section     .text
global      _start

_start:
    push samplestr3
    push samplestr2
    push samplestr1
    push samplefmt0
    call printf
    Exit 0    

printf:
    push        rbp
    mov         rbp, rsp

    xor         r14, r14                    ;r14 is % counter
    ;Calculating length of string
    mov         rsi, [rbp + 16]             ;
    mov         rdi, rsi                    ;
    mov         al, 0x00                    ;
    mov         rcx, 0xFF                   ;Let limit the length of 256 characters
    repne scasb                             ;
    mov         r15, 0xFF                   ;
    sub         r15, rcx                    ;Now length in r15


    mov         rdi, [rbp + 16]             

    ;Searching next %
next_percent:
    mov         rsi, rdi                    ;
    mov         al, '%'                     ;
    mov         rcx, r15                    ;
    repne scasb                             ;
    mov         rdx, r15                    ;
    sub         rdx, rcx                    ;
    dec         rdx                         ;
    mov         rbx, rdi                    ;                                      ;
    mov         rax, 0x01                   ;system call number (sys_write) 
    mov         rdi, 0x01                   ;file descriptor (stdout)
    syscall                                 ;

    sub         r15, rdx
    mov         rdi, rbx
    inc         r14

    cmp         [rdi], byte 0x00
    je          return
    cmp         [rdi], byte 's'
    je          printf_s  

next:
    inc         rdi
    cmp         [rdi], byte 0x00
    je          return
    jmp         next_percent

return:
    pop         rbp
    ret

printf_s: 
    sub         r15, 2
    push        rdi
    push        rcx
    push        rsi
    push        rdx

    ;Calculating length of string
    mov         rsi, [rbp + 16 + r14 * 8]   ;
    mov         rdi, rsi                    ;
    mov         al, 0x00                    ;
    mov         rcx, 0xFF                   ;Let limit the length of 256 characters
    repne scasb                             ;
    mov         rdx, 0xFE                   ;
    sub         rdx, rcx                    ;Now length in rdx

    mov         rax, 0x01                   ;system call number (sys_write) 
    mov         rdi, 0x01                   ;file descriptor (stdout)
    syscall

    pop         rdx
    pop         rsi
    pop         rcx
    pop         rdi

    jmp         next

section     .data
    samplefmt0  db "0123456789%s0123456789=%s Lorem ipsum %s", 0x00
    samplestr1  db "LOL", 0x00
    samplestr2  db "LOL2", 0x00
    samplestr3  db "LOL3", 0x00
