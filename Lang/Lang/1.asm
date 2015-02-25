in
pop ax
in
pop bx
in
pop cx
push cx
push ax
mul
push 4
mul
push 2
push bx
pow
sub
pop dx
push 0
push dx
jb _l1
push 2
push 1
div
push dx
pow
pop dx
push 0
pop cx
:l1
push 2
push cx
ja _l2
push ax
push 2
mul
push dx
push -1
push bx
mul
add
div
out
push ax
push 2
mul
push dx
push -1
push bx
mul
sub
div
out
push cx
push 1
add
pop cx
jmp _l1
:l2
:l3
end