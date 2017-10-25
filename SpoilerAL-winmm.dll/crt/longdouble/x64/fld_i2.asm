public _fld_i2

.code

_fld_i2 proc
	mov     word ptr [rsp + 8], dx
	fild    word ptr [rsp + 8]
	fstp    tbyte ptr [rcx]
	ret
_fld_i2 endp

end
