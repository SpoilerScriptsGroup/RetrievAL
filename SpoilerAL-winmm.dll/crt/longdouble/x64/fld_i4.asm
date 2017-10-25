public _fld_i4

.code

_fld_i4 proc
	mov     dword ptr [rsp + 8], edx
	fild    dword ptr [rsp + 8]
	fstp    tbyte ptr [rcx]
	ret
_fld_i4 endp

end
