public _fld_i8

.code

_fld_i8 proc
	mov     qword ptr [rsp + 8], rdx
	fild    qword ptr [rsp + 8]
	fstp    tbyte ptr [rcx]
	ret
_fld_i8 endp

end
