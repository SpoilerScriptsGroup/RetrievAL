public _fst_i8

.code

_fst_i8 proc
	fld     tbyte ptr [rcx]
	fistp   qword ptr [rsp + 8]
	mov     rax, qword ptr [rsp + 8]
	ret
_fst_i8 endp

end
