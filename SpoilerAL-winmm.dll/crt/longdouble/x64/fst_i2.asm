public _fst_i2

.code

_fst_i2 proc
	fld     tbyte ptr [rcx]
	fistp   word ptr [rsp + 8]
	mov     ax, word ptr [rsp + 8]
	ret
_fst_i2 endp

end
