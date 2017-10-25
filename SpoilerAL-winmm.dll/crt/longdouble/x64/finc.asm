public _finc

.code

_finc proc
	fld     tbyte ptr [rdx]
	fld1
	fadd
	fstp    tbyte ptr [rcx]
	ret
_finc endp

end
