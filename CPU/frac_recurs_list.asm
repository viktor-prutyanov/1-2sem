in
call _frac
end
:frac
dup
push 1
je _l1
dup
push 1
sub
call _frac
mul
dup
out
:l1
ret