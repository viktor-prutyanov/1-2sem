in
call _frac
out
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
:l1
ret