;Exit program with code
;Out: None
;Dstr: RAX, RDI
%macro Exit 1
    mov         rax, 0x3c                   ;system call number (sys_exit)
    mov         rdi, %1                              
    syscall
%endmacro

;Reads char from stdout
;Out: None
;Dstr: RAX, RDI
%macro Read_char 2
    mov         rax, 0x00                   ;system call number (sys_read)
    mov         rdx, %2
    mov         rdi, 0x00                   ;file descriptor (stdin)
    mov         rsi, %1                          
    syscall
%endmacro

;Prints string in stdout
;Out: None
;Dstr: RAX, RDI, RSI, RDX
%macro Print 2
    mov         rsi, %1
    mov         rax, 0x01                   ;system call number (sys_write)                  
    mov         rdi, 0x01                   ;file descriptor (stdout)
    mov         rdx, %2
    syscall
%endmacro

section .text
global _start

_start:
    Read_char   text, 1
    mov         r8, rax        
    
    mov         rsi, text
    mov         rax, 0x01                   ;system call number (sys_write)                  
    mov         rdi, 0x01                   ;file descriptor (stdout)
    mov         rdx, r8
    syscall
Exit 0

section .data
text        times 0xFF db 0x00
text_len    equ 0xFF
chr         db 0x56