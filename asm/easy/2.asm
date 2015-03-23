;   ASCII code checker by parity 
;
;   @date 02.2015
;
;   @copyright GNU GPL v2.0
;
;   @author Viktor Prutyanov mailto:vitteran@gmail.com SECTION .text
org 0x100
    mov     dx, request
    mov     ah, 0x09
    int     0x21        ;the request printed

    mov     ah, 0x01
    int     0x21
    sub     al, 0x30
    mov     cl, 0x64
    mul     cl
    mov     bl, al

    mov     ah, 0x01
    int     0x21
    sub     al, 0x30
    mov     cl, 0x0A
    mul     cl
    add     bl, al

    mov     ah, 0x01
    int     0x21
    sub     al, 0x30
    add     bl, al

    cmp     bl, 0x30    ;comparsion with 0 character
    jb      m_no_digit
    cmp     bl, 0x39    ;comparsion with 9 character
    ja      m_no_digit

    and     bl, 0x01

    cmp     bl, 0x01
    je      m_even

m_odd: 
    mov     dx, odd
    mov     ah, 0x09
    int     0x21        ;the odd response printed
    jmp     m_end

m_even:
    mov     dx, even
    mov     ah, 0x09
    int     0x21        ;the even response printed

m_end:
    mov     ax, 0x4c00
    int     0x21

m_no_digit:
    mov     dx, no_digit
    mov     ah, 0x09
    int     0x21
    jmp     m_end       ;the no_digit response printed

SECTION .data
    request:  db "Please enter ASCII code of a digit (strictly 3 digits in number):", 0xD, 0xA, '$'
    odd:      db 0xD, 0xA, "You entered ASCII code of odd digit.", 0xD, 0xA, '$'
    even:     db 0xD, 0xA, "You entered ASCII code of even digit.", 0xD, 0xA, '$'
    no_digit: db 0xD, 0xA, "Character you have entered is not a digit.", 0xD, 0xA, '$'