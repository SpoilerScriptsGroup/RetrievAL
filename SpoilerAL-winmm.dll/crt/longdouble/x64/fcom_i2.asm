public _fcom_i2

.code

_fcom_i2 proc
	mov     word ptr [rsp + 8], dx
	fild    word ptr [rsp + 8]
	fld     tbyte ptr [rcx]
	fcompp
	fstsw   ax
	and     ax, 4300H
	ret
_fcom_i2 endp

end
