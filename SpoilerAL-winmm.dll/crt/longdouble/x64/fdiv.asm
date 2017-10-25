public _fdiv

.code

_fdiv proc
	fld     tbyte ptr [rdx]
	fld     tbyte ptr [r8]
	fdiv
	fstp    tbyte ptr [rcx]
	ret
_fdiv endp

end
