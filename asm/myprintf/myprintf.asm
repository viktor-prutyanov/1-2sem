;   Custom printf realization
;   03.2015
;   GNU GPL v2.0
;   Viktor Prutyanov mailto:vitteran@gmail.com

%define MAX_LEN 0xFF
%define NULL    0x00
%define BUF_LEN 0xFF

;Exit program with code
;Out: None
;Dstr: RAX, RDI
%macro Exit 1
    mov         rax, 0x3c                   ;system call number (sys_exit)
    mov         rdi, %1                     ;exitcode   
    syscall
%endmacro

bits 64

section     .text
global      myprintf

;###############################################
;# NUMBER MUST BE LONG (1337L instead of 1337) #
;###############################################
;
;prototype: void __cdecl _myprintf(const char *, ...);
myprintf:
    push        rbp
    mov         rbp, rsp

    ; rdi, rsi, rdx, rcx, r8, r9
    push        rdi
    push        rsi
    push        rdx
    push        rcx
    push        r8
    push        r9

    xor         r14, r14                    ;r14 is % counter
    ;Calculating length of string
    mov         rsi, [rbp - 8]              ;
    mov         rdi, rsi                    ;
    mov         al,  NULL                   ;
    mov         rcx, MAX_LEN                ;Let limit the length of 256 characters
    repne scasb                             ;
    mov         r15, MAX_LEN                ;
    sub         r15, rcx                    ;Now length in r15


    mov         rdi, [rbp - 8]             

    ;Searching next %
next_percent:
    mov         rsi, rdi                    ;
    mov         al,  '%'                    ;
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
    cmp         [rdi], byte 'd'
    je          printf_d
    cmp         [rdi], byte 'c'
    je          printf_c
    cmp         [rdi], byte 'b'
    je          printf_b
    cmp         [rdi], byte 'o'
    je          printf_o
    cmp         [rdi], byte 'x'
    je          printf_x
    cmp         [rdi], byte '%'
    je          printf_prcnt

next:
    inc         rdi
    cmp         [rdi], byte 0x00
    je          return
    jmp         next_percent

return:
    pop         r9
    pop         r8
    pop         rcx
    pop         rdx
    pop         rsi
    pop         rdi

    pop         rbp
    ret

;Percent handler
printf_prcnt:
    dec         r14
    push        rdi
    push        rsi
    push        rdx

    sub         r15, 2

    mov         [buffer], byte '%'
    mov         rax, 0x01                   ;system call number (sys_write) 
    mov         rdi, 0x01                   ;file descriptor (stdout)
    mov         rsi, buffer
    mov         rdx, 1
    syscall

    pop         rdx
    pop         rsi
    pop         rdi

    jmp         next

;String handler
printf_s: 
    push        rdi
    push        rsi
    push        rdx

    sub         r15, 2

    mov         r13, r14
    shl         r13, 3
    add         r13, 8
    mov         r12, rbp
    sub         r12, r13

    mov         rsi, [r12]                  ;[rbp - 8 - r14 * 8]    ;
    cmp         r14, 6
    jb          through_registers3
    mov         rsi, [rbp - 32 + r14 * 8]
through_registers3:

    ;Calculating length of string
    mov         rdi, rsi                    ;
    mov         al,  NULL                   ;
    mov         rcx, MAX_LEN                ;Let limit the length of 256 characters
    repne scasb                             ;
    mov         rdx, MAX_LEN - 1            ;
    sub         rdx, rcx                    ;Now length in rdx

    mov         rax, 0x01                   ;system call number (sys_write) 
    mov         rdi, 0x01                   ;file descriptor (stdout)
    syscall

    pop         rdx
    pop         rsi
    pop         rdi

    jmp         next

;Binary number handler
printf_b:
    mov         rcx, 1
    jmp         printf_bx
;Octal number handler
printf_o:
    mov         rcx, 3
    jmp         printf_bx
;Hexadecimal number handler
printf_x:
    mov         rcx, 4

;2^n number handler
printf_bx:
    push        rdi
    push        rsi
    push        rdx

    sub         r15, 2
    xor         rbx, rbx

    mov         r13, r14
    shl         r13, 3
    add         r13, 8
    mov         r12, rbp
    sub         r12, r13

    mov         rax, [r12]                  ;[rbp - 8 - r14 * 8]
    cmp         r14, 6
    jb          through_registers2
    mov         rax, [rbp - 32 + r14 * 8]
through_registers2:

next_div_b:  
    cmp         rbx, BUF_LEN
    je          next_digit_b  
    mov         rdx, 1
    shl         rdx, cl
    sub         rdx, 1                      ;now DX is 2^CL - 1 = 2^n-1 
    and         rdx, rax                    ;remainder AX/2^n is in DX
    shr         rax, cl                     ;AX = AX/2^n
    inc         rbx
    push        rdx
    cmp         rax, 0
    jne         next_div_b

    mov         rsi, buffer
    mov         rdi, rsi

    mov         rcx, rbx
next_digit_b:
    pop         rax
    cmp         rax, 10
    jb          digit
    add         rax, 'A' - 10 - '0'
digit:
    add         rax, '0'
    stosb
    loop        next_digit_b

    mov         rdx, rdi
    sub         rdx, rsi
    mov         rax, 0x01                   ;system call number (sys_write) 
    mov         rdi, 0x01                   ;file descriptor (stdout)
    syscall

    pop         rdx
    pop         rsi
    pop         rdi

    jmp         next

;Char handler
printf_c:
    push        rdi
    push        rsi
    push        rdx

    sub         r15, 2

    mov         r13, r14
    shl         r13, 3
    add         r13, 8
    mov         r12, rbp
    sub         r12, r13

    mov         rax, [r12]                  ;[rbp - 8 - r14 * 8]
    cmp         r14, 6
    jb          through_registers4
    mov         rax, [rbp - 32 + r14 * 8]
through_registers4:

    
    mov         [buffer], al
    mov         rax, 0x01                   ;system call number (sys_write) 
    mov         rdi, 0x01                   ;file descriptor (stdout)
    mov         rsi, buffer
    mov         rdx, 1
    syscall

    pop         rdx
    pop         rsi
    pop         rdi

    jmp         next

;Decimal number handler
printf_d:
    push        rdi
    push        rsi
    push        rdx

    sub         r15, 2
    xor         rcx, rcx

    mov         r13, r14
    shl         r13, 3
    add         r13, 8
    mov         r12, rbp
    sub         r12, r13

    mov         rax, [r12]                    ;[rbp - 8 - r14 * 8]
    cmp         r14, 6
    jb          through_registers1
    mov         rax, [rbp - 32 + r14 * 8]
through_registers1:
    
    mov         rbx, 10
    
next_div:    
    cmp         rcx, BUF_LEN
    je          next_digit_d
    inc         rcx
    xor         rdx, rdx
    div         rbx
    push        rdx
    cmp         rax, 0
    jne         next_div

    mov         rsi, buffer
    mov         rdi, rsi

next_digit_d:
    pop         rax
    add         rax, '0'
    stosb
    loop        next_digit_d

    mov         rdx, rdi
    sub         rdx, rsi
    mov         rax, 0x01                   ;system call number (sys_write) 
    mov         rdi, 0x01                   ;file descriptor (stdout)
    syscall

    pop         rdx
    pop         rsi
    pop         rdi

    jmp         next

section     .data
    buffer      times BUF_LEN db 0x00