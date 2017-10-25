public _fldcw

.code

_fldcw proc
	mov     word ptr [rsp + 8], cx
	fldcw   word ptr [rsp + 8]
	ret
_fldcw endp

end
