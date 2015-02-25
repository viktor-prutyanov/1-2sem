SECTION .text
org 0x100
    mov dx, request
    mov ah, 0x09
    int 0x21
    ;the request printed

    mov ah, 0x01
    int 0x21
    ;now the character is in AL

    cmp al, 0x30 ;comparsion with zero character
    je m_zero

    mov dx, not_zero
    mov ah, 0x09
    int 0x21
    ;the not_zero response printed

    jmp end

    m_zero:
    mov dx, zero
    mov ah, 0x09
    int 0x21
    ;the zero response printed

    end:
    mov ax, 0x4c00
    int 0x21
SECTION .data
	request: 	db "Please enter one character:", 0xD, 0xA, '$'
    zero: 		db 0xD, 0xA,"User has entered 0.", 0xD, 0xA, '$'
    not_zero: 	db 0xD, 0xA,"User has entered not 0.", 0xD, 0xA, '$'