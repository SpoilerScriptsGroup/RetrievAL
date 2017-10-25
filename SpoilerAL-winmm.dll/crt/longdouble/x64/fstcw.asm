public _fstcw

.code

_fstcw proc
	fstcw   word ptr [rsp + 8]
	mov     ax, word ptr [rsp + 8]
	ret
_fstcw endp

end
