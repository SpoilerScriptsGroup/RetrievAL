public _fst_i4

.code

_fst_i4 proc
	fld     tbyte ptr [rcx]
	fistp   dword ptr [rsp + 8]
	mov     eax, dword ptr [rsp + 8]
	ret
_fst_i4 endp

end
