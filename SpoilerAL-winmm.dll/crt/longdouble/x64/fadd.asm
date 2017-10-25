public _fadd

.code

_fadd proc
	fld     tbyte ptr [rdx]
	fld     tbyte ptr [r8]
	fadd
	fstp    tbyte ptr [rcx]
	ret
_fadd endp

end
