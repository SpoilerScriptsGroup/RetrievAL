public _fst_r8

.code

_fst_r8 proc
	fld     tbyte ptr [rcx]
	fstp    qword ptr [rsp + 8]
	movupd  xmm0, qword ptr [rsp + 8]
	ret
_fst_r8 endp

end
