public _fld1

.code

_fld1 proc
	fld1
	fstp    tbyte ptr [rcx]
	ret
_fld1 endp

end
