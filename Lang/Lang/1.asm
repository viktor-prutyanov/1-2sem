in
pop ax
push 1
push ax
jb _l1
push 1
pop bx
push 1
pop cx
:l1
push ax
push cx
ja _l2
push cx
push bx
mul
pop bx
push cx
push 1
add
pop cx
jmp _l1
:l2
:l3
push 1
push ax
jae _l4
push -1
pop bx
:l4
push bx
out
end