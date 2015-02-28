SECTION .text
org 0x100
    mov ah, 0x9
    mov dx, plz_msg
    int 0x21

    mov edi, key
    xor cx, cx

lp:
    mov ah, 0x01
    int 0x21    ;char now in AL

    xor al, 0x0D 
    jz check
    xor al, 0x0D

    xor [edi], byte '$'
    jz idle
    xor [edi], byte '$'

    xor al, [edi]
    jz next
    mov cx, 0xDEAD
next:
    inc edi
idle:
    jmp lp

check:
    xor cx, 0xDEAD
    jz ad
    xor [edi], byte '$'
    jz ok

ad:
    mov ah, 0x9
    mov dx, ad_msg
    int 0x21

end:
    mov ax, 0x4c00
    int 0x21
    
ok: 
    mov ah, 0x9
    mov dx, ok_msg
    int 0x21
    jmp end

SECTION .data
    plz_msg: db "PLEASE ENTER PASSWORD:", 0xD, 0xA, '$'
    key:     db "PASSWORD1337", '$'
    ok_msg:  db 0xD, 0xA, "OK", '$'
    ad_msg:  db 0xD, 0xA, "ACCESS DENIED", '$'