public _fxtract

.code

_fxtract proc
	fld     tbyte ptr [rdx]
	fxtract
	fstp    tbyte ptr [rcx]
	mov     rax, qword ptr [r8]
	fstp    tbyte ptr [rax]
	ret
_fxtract endp

end
