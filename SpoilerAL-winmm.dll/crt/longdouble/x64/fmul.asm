public _fmul

.code

_fmul proc
	fld     tbyte ptr [rdx]
	fld     tbyte ptr [r8]
	fmul
	fstp    tbyte ptr [rcx]
	ret
_fmul endp

end
