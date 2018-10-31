public _fld_u8

.code

_fld_u8 proc
	mov     qword ptr [rsp + 8], rdx
	mov     word ptr [rsp + 16], 403EH
	test    rdx, rdx
	js      L1
	fild    qword ptr [rsp + 8]
	jmp     L2
L1:
	fld     tbyte ptr [rsp + 8]
L2:
	fstp    tbyte ptr [rcx]
	ret
_fld_u8 endp

end
