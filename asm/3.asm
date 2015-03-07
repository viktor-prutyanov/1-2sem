;   Password checker
;
;   @date 02.2015
;
;   @copyright GNU GPL v2.0
;
;   @author Viktor Prutyanov mailto:vitteran@gmail.com 

SECTION .text
org 0x100 
    mov ah, 0x9
    mov dx, plz
    int 0x21

    xor cx, cx
    xor bx, bx
loop_m:
    mov ah, 0x01
    int 0x21 ;char is in AL
    cmp al, 0x0d 
    je read_end
    sub al, 0x30
    xor ah, ah
    push ax
    inc cl
    jmp loop_m

read_end:
    pop ax
    call pow10
    add bx, ax
    inc ch
    cmp cl, ch
    jne read_end

    ;number is in BX now

    mov ax, bx
    mov cx, 0x0002
    xor dx, dx
    div cx ;ax = bx/2
    xor dx, 0x0000
    mov cx, 0x0001
    mov si, ax
    jz not_prime_m

next_check:
    mov ax, bx
    add cx, 0x0002
    xor dx, dx
    div cx
    xor dx, 0x0000
    jz not_prime_m
    cmp cx, si
    ja prime_m
    jmp next_check

prime_m:     
    mov ah, 0x9
    mov dx, prime
    int 0x21
    jmp end

not_prime_m:
    mov ah, 0x9
    mov dx, not_prime
    int 0x21

end:    
    mov ax, 0x4c00
    int 0x21 

pow10: ;dx:ax = ax * 10^ch, ch >= 1
    push cx
next:
    cmp ch, 0x00
    je return
    mov dx, 0x000a
    mul dx
    dec ch
    jmp next
return:
    pop cx
    ret  

SECTION .data
    plz:        db "Please enter number (up to 255):", 0xD, 0xA, '$' 
    prime:      db 0xD, 0xA, "Number is prime",     '$'
    not_prime:  db 0xD, 0xA, "Number is not prime", '$'
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