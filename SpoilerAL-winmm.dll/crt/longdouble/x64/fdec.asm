public _fdec

.code

_fdec proc
	fld     tbyte ptr [rdx]
	fld1
	fsub
	fstp    tbyte ptr [rcx]
	ret
_fdec endp

end
