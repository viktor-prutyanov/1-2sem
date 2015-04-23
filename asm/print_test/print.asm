;   ; Printf() call from assembly test
;   ; 03.2015
;   ; GNU GPL v2.0
;   ; Viktor Prutyanov mailto:vitteran@gmail.com

; bits 64

; global main

; extern printf

; section .text
; main:
;     mov         r9, 24              ;5 arg
;     mov         r8, 16              ;4 arg
;     mov         rcx, 8              ;3 arg
;     mov         rdx, 3802           ;2 arg
;     mov         rsi, vp_str         ;1 arg
;     mov         rdi, fmt_str        ;0 arg
;     sub         rsp, 8
;     push        vp_str
;     push        qword 64            ;10 arg
;     push        qword 56            ;9 arg
;     push        qword 48            ;8 arg
;     push        qword 40            ;7 arg
;     push        qword 32            ;6 arg
;     call        printf
;     add         rsp, 56                                                                                                                                                                         

;     mov         rax, 0
;     ret

; section .data
;     fmt_str:    db "Hello from %s, 0x%X forever! %X %X %X %X %X %X %X %X %s", 0x00
;     vp_str:     db "V.P.!", 0x00

  ; Printf() call from assembly test
  ; 03.2015
  ; GNU GPL v2.0
  ; Viktor Prutyanov mailto:vitteran@gmail.com

bits 64

global main

extern printf

section .text
main:
    mov         rcx, 8              ;3 arg
    mov         rdx, 3802           ;2 arg
    mov         rsi, vp_str         ;1 arg
    mov         rdi, fmt_str        ;0 arg
    sub         rsp, 8
    call        printf                                                                                                                                                                      
    add         rsp, 8

    mov         rax, 0
    ret

section .data
    fmt_str:    db "Hello from %s, 0x%X forever! %X", 0x00
    vp_str:     db "V.P.!", 0x00