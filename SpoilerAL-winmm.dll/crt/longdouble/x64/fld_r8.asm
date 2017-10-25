public _fld_r8

.code

_fld_r8 proc
	movsd   qword ptr [rsp + 8], xmm1
	fld     qword ptr [rsp + 8]
	fstp    tbyte ptr [rcx]
	ret
_fld_r8 endp

end
