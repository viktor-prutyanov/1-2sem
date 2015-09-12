in
pop ax
call _fact
push bx
out
end
:fact
push ax
push 1
je _one
push ax
push ax
push 1
sub
pop ax
call _fact
push bx
mul
pop bx
ret
:one
mov bx 1
ret