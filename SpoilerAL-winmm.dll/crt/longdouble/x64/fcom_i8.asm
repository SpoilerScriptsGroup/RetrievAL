public _fcom_i8

.code

_fcom_i8 proc
	mov     qword ptr [rsp + 8], rdx
	fild    qword ptr [rsp + 8]
	fld     tbyte ptr [rcx]
	fcompp
	fstsw   ax
	and     ax, 4300H
	ret
_fcom_i8 endp

end
