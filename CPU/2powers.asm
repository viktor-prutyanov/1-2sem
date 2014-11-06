mov ax 2
push 1000
push 1
:label0
push ax
mul
pop ax
push ax
out
push ax
push 1000
push ax
jb _label0
end