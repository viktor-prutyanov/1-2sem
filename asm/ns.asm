;   2,8,10,16 numeral systems translator
;   02.2015-03.2015
;   GNU GPL v2.0
;   Viktor Prutyanov mailto:vitteran@gmail.com

;Prints string in stdout
;Out: AL = 0x24
;Dstr: AH, DX
%macro Print 1
    mov     ah, 0x9
    mov     dx, %1
    int     0x21
%endmacro

;Exit program with code
;Out: None
;Dstr: AX
%macro Exit 1
    mov     ah, 0x4c
    mov     al, %1
    int     0x21 
%endmacro

SECTION .text
org 0x100 
    xor     ebp, ebp

    Print   plz2
    mov     ah, 0x01
    int     0x21        ;char is in AL

    mov     bl, al
    Print   plz1
    cmp     bl, 'b'
    je      bin1_m
    cmp     bl, 'o'
    je      oct1_m
    cmp     bl, 'd'
    je      dec1_m
    cmp     bl, 'h'
    je      hex1_m
    Print   err1
    Exit    1

bin1_m:
    push    0x01
    call    binx_read
    pop     dx
    jmp     output
oct1_m:
    push    0x03
    call    binx_read
    pop     dx
    jmp     output
dec1_m:
    call    dec_read
    jmp     output
hex1_m:
    push    0x04
    call    binx_read
    pop     dx

output:
    Print   plz2
    mov     ah, 0x01
    int     0x21        ;char is in AL

    cmp     al, 'b'
    je      bin2_m
    cmp     al, 'o'
    je      oct2_m
    cmp     al, 'd'
    je      dec2_m
    cmp     al, 'h'
    je      hex2_m
    Print   err1
    Exit    1

bin2_m:
    Print   CRLF
    push    bx
    push    0x01
    call    binx_write
    jmp     end
oct2_m:
    Print   CRLF
    push    bx
    push    0x03
    call    binx_write
    jmp     end
dec2_m:
    Print   CRLF
    push    bx
    push    0x0A
    call    all_write
    jmp     end
hex2_m:
    Print   CRLF
    push    bx
    push    0x04
    call    binx_write

    end:
    pop     bx
    pop     bx
    Exit    0

;#########################################################
;# # # # # # # # # # # # # # # # # # # # # # # # # # # # #
;#########################################################

;AX = arg1 * 10^arg2
;In: arg1 <- multiplier, arg2 <- degree
;Dstr: None
;Out: AX
pow10:
    push    ebp 
    mov     ebp, esp
    mov     ax, [bp + 8] ;arg1
    mov     cx, [bp + 6] ;arg2

    push    bx
    push    dx
    mov     bx, 0x0A
    or      cx, 0x00   
    jz      m2          ;if degree is 0, nothing to do
m1:
    mul     bx
    loop    m1
m2:
    pop     dx
    pop     bx

    pop     ebp     
    ret

;Read number from stdin in 2^n numeral system
;In: arg1 <- n
;Dstr: AX, CX
;Out: BX
binx_read:
    push    ebp 
    mov     ebp, esp
    mov     dx, [ebp + 6] ;arg1

    xor     bx, bx
    xor     cx, cx

binx_read_loop1:
    mov     ah, 0x01
    int     0x21 ;char is in AL
    cmp     al, 0x0d 
    je      binx_read_loop2

    cmp     al, 'a'                 ;Check letter or not,
    jb      digit                   ;then jump 
    sub     al, 'a' - '0' - 10      ;or make it letter
digit:     
    sub     al, '0'

    xor     ah, ah
    push    ax
    add     ch, dl
    jmp     binx_read_loop1

binx_read_loop2:
    pop     ax
    shl     ax, cl
    add     bx, ax
    add     cl, dl
    cmp     ch, cl
    jne     binx_read_loop2

    pop     ebp
    ret

;Read number from stdin in 10 numeral system
;In: None
;Dstr: AX, CX, BX
;Out: BX
dec_read:
    push    ebp 
    mov     ebp, esp

    xor     bx, bx
    xor     cx, cx
    xor     dx, dx

dec_read_loop1:
    mov     ah, 0x01
    int     0x21 ;char is in AL
    cmp     al, 0x0d 
    je      dec_read_loop2
    sub     al, '0'
    xor     ah, ah
    push    ax
    inc     dx
    jmp     dec_read_loop1

dec_read_loop2:
    ;ax wasn't pushed because it is in stack already
    push    cx          
    call    pow10
    pop     cx
    add     bx, ax
    pop     ax
    inc     cx
    cmp     cx, dx
    jne     dec_read_loop2

    pop     ebp
    ret

;Write number to stdout in all (up to 36) numeral systems
;In: arg1 <- number, arg2 <- system
;Dstr: CX, DX
;Out: None
all_write:
    push    ebp 
    mov     ebp, esp
    mov     ax, [ebp + 8]   ;arg1
    mov     bx, [ebp + 6]   ;arg2

    xor     dx, dx
    xor     cx, cx

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
    mov     ah, 0x02
    int     0x21
    loop    all_write_loop2

    pop     ebp
    ret

;Write number to stdout in 2^n numeral systems
;In: arg1 <- number, arg2 <- n
;Dstr: DX, BX
;Out: None
binx_write:
    push    ebp
    mov     ebp, esp
    mov     ax, [ebp + 8]   ;arg1
    mov     cx, [ebp + 6]   ;arg2

    xor     dx, dx
    xor     bx, bx

binx_write_loop1:
    mov     dx, 0x01
    shl     dx, cl
    sub     dx, 0x01         ;now DX is 2^CL - 1 = 2^n-1 
    and     dx, ax           ;remainder AX/2^n is in DX
    shr     ax, cl           ;AX = AX/2^n

    add     dx, '0'
    push    dx
    inc     bx
    or      ax, 0x00
    jz      binx_write_pre_loop2
    jmp     binx_write_loop1

binx_write_pre_loop2:
    mov     cx, bx
binx_write_loop2:
    pop     dx
    cmp     dx, '9' + 1             ;Check letter or not,
    jb      digit3                  ;then jump 
    add     dx, 'a' - '0' - 10      ;or make it letter
digit3:
    mov     ah, 0x02
    int     0x21
    loop    binx_write_loop2

    pop     ebp
    ret

SECTION .data
    CRLF:   db 0xD, 0xA, '$'
    plz1:   db 0xD, 0xA, "Please enter number (letters in hex must be small):", 0xD, 0xA, '$'
    plz2:   db 0xD, 0xA, "Please enter b|o|d|h to choose numeral system:", 0xD, 0xA, '$' 
    err1:   db 0xD, 0xA, "ERROR:Undefined numeral system", 0xD, 0xA, '$'