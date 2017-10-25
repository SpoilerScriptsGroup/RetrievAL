public _fcom_i4

.code

_fcom_i4 proc
	mov     dword ptr [rsp + 8], edx
	fild    dword ptr [rsp + 8]
	fld     tbyte ptr [rcx]
	fcompp
	fstsw   ax
	ret
_fcom_i4 endp

end
