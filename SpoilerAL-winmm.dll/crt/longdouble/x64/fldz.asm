public _fldz

.code

_fldz proc
	fldz
	fstp    tbyte ptr [rcx]
	ret
_fldz endp

end
