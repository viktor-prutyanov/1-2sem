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
:l1
push 0
push dx
jb _l2
push 2
push 1
div
push dx
pow
pop dx
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
jmp _l1
:l2
end