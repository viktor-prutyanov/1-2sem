push 2
pop ax
push 1000
push 1
:0
push ax
mul
pop ax
push ax
out
push ax
push 1000
push ax
jb :0
end