public _fabs

.code

_fabs proc
	fld     tbyte ptr [rdx]
	fabs
	fstp    tbyte ptr [rcx]
	ret
_fabs endp

end
