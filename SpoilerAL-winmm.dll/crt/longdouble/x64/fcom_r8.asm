public _fcom_r8

.code

_fcom_r8 proc
	fld     tbyte ptr [rdx]
	movsd   qword ptr [rsp + 8], xmm2
	fld     qword ptr [rsp + 8]
	fmul
	fstp    tbyte ptr [rcx]
	ret
_fcom_r8 endp

end
