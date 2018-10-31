public _fcom_u8

.code

_fcom_u8 proc
	mov     qword ptr [rsp + 8], rdx
	mov     word ptr [rsp + 16], 403EH
	test    rdx, rdx
	js      L1
	fild    qword ptr [rsp + 8]
	jmp     L2
L1:
	fld     tbyte ptr [rsp + 8]
L2:
	fld     tbyte ptr [rcx]
	fcompp
	fstsw   ax
	and     ax, 4300H
	ret
_fcom_u8 endp

end
