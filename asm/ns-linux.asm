;   2,8,10,16 numeral systems translator
;   02.2015-03.2015
;   GNU GPL v2.0
;   Viktor Prutyanov mailto:vitteran@gmail.com

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

;Exit program with code
;Out: None
;Dstr: RAX, RDI
%macro Exit 1
    mov         rax, 0x3c                   ;system call number (sys_exit)
    mov         rdi, %1                              
    syscall
%endmacro

;Reads char from stdout
;Out: RAX <- amount of readed symbols
;Dstr: RDI, RSI, RDX
%macro Read 2
    mov         rax, 0x00                   ;system call number (sys_read)
    mov         rdx, %2
    mov         rdi, 0x00                   ;file descriptor (stdin)
    mov         rsi, %1                          
    syscall
%endmacro

section     .text
global      main

main:
    Print       plz2, plz2_len
    Read        buf, buf_len
    mov         bl, [buf]

    Print       plz1, plz1_len
    cmp         bl, 'b'
    je          bin1_m
    cmp         bl, 'o'
    je          oct1_m
    cmp         bl, 'd'
    je          dec1_m
    cmp         bl, 'h'
    je          hex1_m
    Print       err, err_len
    Exit        0

bin1_m:
    Print       bin_msg, 4
    ;mov         dl, 0x01
    call        binx_read
    jmp         output
oct1_m:
    ; mov     dl, 0x03
    ; call    binx_read
    Print       oct_msg, 4
    jmp         output
dec1_m:
    ;call    dec_read
    Print       dec_msg, 4
    jmp         output
hex1_m:
    Print       hex_msg, 4
    ;mov     dl, 0x04
    ;call    binx_read

output:
    mov         ax, dx
    mov         bx, 10
    call        all_write
    Print       text, r8
    Exit        0
;     Print   plz2
;     mov     ah, 0x01
;     int     0x21        ;char is in AL

;     cmp     al, 'b'
;     je      bin2_m
;     cmp     al, 'o'
;     je      oct2_m
;     cmp     al, 'd'
;     je      dec2_m
;     cmp     al, 'h'
;     je      hex2_m
;     Print   err1
;     Exit    1

; bin2_m:
;     Print   CRLF
;     mov     ax, bx
;     mov     cl, 0x01
;     call    binx_write
;     jmp     end
; oct2_m:
;     Print   CRLF
;     mov     ax, bx
;     mov     cl, 0x03
;     call    binx_write
;     jmp     end
; dec2_m:
;     Print   CRLF
;     mov     ax, bx
;     mov     bx, 0x0A
;     call    all_write
;     jmp     end
; hex2_m:
;     Print   CRLF
;     mov     ax, bx
;     mov     cl, 0x04
;     call    binx_write

;     end:
;     Exit    0

; ;#########################################################
; ;# # # # # # # # # # # # # # # # # # # # # # # # # # # # #
; ;#########################################################

;DX:AX = AX * 10^CX
;In: AX <- multiplier, CX <- degree
;Dstr: None
;Out: DX:AX
; pow10:
;     push    cx
;     push    bx
;     push    dx
;     mov     bx, 0x0A
;     or      cx, 0x00   
;     jz      m2          ;if degree is 0, nothing to do
; m1:
;     mul     bx
;     loop    m1
; m2:
;     pop     dx
;     pop     bx
;     pop     cx
;     ret

;Read number from stdin in 2^n numeral system
binx_read:
    Read        text, text_len
    mov         r8, rax                 ;rax is real length
    Print       text, r8
    xor         rbx, rbx                ;rbx is counter
    xor         rax, rax
    xor         rcx, rcx
    xor         rdx, rdx

binx_read_loop:
    mov         al, byte [text + rbx]
    cmp         al, 'a'                 ;Check letter or not,
    jb          digit                   ;then jump 
    sub         al, 'a' - '0' - 10      ;or make it letter
digit:     
    sub         al, '0'

    mov         rcx, r8
    inc         rbx
    sub         rcx, rbx
    shl         rax, cl
    add         rdx, rax
    cmp         rbx, r8
    jne         binx_read_loop           
    ret
; binx_read_loop2:
;     pop     ax
;     shl     ax, cl
;     add     bx, ax
;     add     cl, dl
;     cmp     ch, cl
;     jne     binx_read_loop2

; ;Read number from stdin in 10 numeral system
; ;In: None
; ;Dstr: AX, CX, BX
; ;Out: BX
; dec_read:
;     xor     bx, bx
;     xor     cx, cx
;     xor     dx, dx

; dec_read_loop1:
;     mov     ah, 0x01
;     int     0x21 ;char is in AL
;     cmp     al, 0x0d 
;     je      dec_read_loop2
;     sub     al, '0'
;     xor     ah, ah
;     push    ax
;     inc     dx
;     jmp     dec_read_loop1

; dec_read_loop2:
;     pop     ax
;     call    pow10
;     add     bx, ax
;     inc     cx
;     cmp     cx, dx
;     jne     dec_read_loop2
;     ret

;Write number to stdout in all (up to 36) numeral systems
all_write:
    xor     dx, dx
    xor     cx, cx
    xor     r8, r8

all_write_loop1:
    xor     dx, dx
    div     bx
    add     dx, '0'
    push    dx
    inc     cx
    or      ax, 0x00
    jz      all_write_loop2
    jmp     all_write_loop1

all_write_loop2:
    pop     dx
    cmp     dx, '9' + 1             ;Check letter or not,
    jb      digit2                  ;then jump 
    add     dx, 'a' - '0' - 10      ;or make it letter
digit2:
    mov     [text + r8], dx
    inc     r8
    loop    all_write_loop2
    ret

; ;Write number to stdout in 2^n numeral systems
; ;In: AX <- number, CL <- n
; ;Dstr: DX, BX
; ;Out: None
; binx_write:
;     xor     dx, dx
;     xor     bx, bx

; binx_write_loop1:
;     mov     dx, 0x01
;     shl     dx, cl
;     sub     dx, 0x01         ;now BX is 2^CL - 1 = 2^n-1 
;     and     dx, ax           ;remainer AX/2^n is in BX
;     shr     ax, cl           ;AX = AX/2^n

;     add     dx, '0'
;     push    dx
;     inc     bx
;     or      ax, 0x00
;     jz      binx_write_pre_loop2
;     jmp     binx_write_loop1

; binx_write_pre_loop2:
;     mov     cx, bx
; binx_write_loop2:
;     pop     dx
;     cmp     dx, '9' + 1             ;Check letter or not,
;     jb      digit3                  ;then jump 
;     add     dx, 'a' - '0' - 10      ;or make it letter
; digit3:
;     mov     ah, 0x02
;     int     0x21
;     loop    binx_write_loop2
;     ret

section     .data
    CRLF        db 0xD, 0xA
    bin_msg     db "Bin:"
    oct_msg     db "Oct:"
    dec_msg     db "Dec:"
    hex_msg     db "Hex:"
    plz1        db "Please enter number (letters in hex must be small).", 0xD, 0xA
    plz1_len    equ $ - plz1    
    plz2        db "Please enter b|o|d|h to choose numeral system:", 0xD, 0xA
    plz2_len    equ $ - plz2
    buf         times 255 db 0x00
    buf_len     equ 255
    text        times 255 db 0x00  
    text_len    equ 255
    err:        db 0xD, 0xA, "ERROR: Undefined numeral system", 0xD, 0xA
    err_len     equ $ - err