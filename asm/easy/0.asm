SECTION .text
org 0x100
    mov ah, 0x9
    mov dx, hello
    int 0x21
    mov ax, 0x4c00
    int 0x21
SECTION .data
    hello: db "Hello, world!", 0xD, 0xA, '$'