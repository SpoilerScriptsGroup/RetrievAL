public _fcom_r8

.code

_fcom_r8 proc
	fld     qword ptr [rdx]
	fld     tbyte ptr [rcx]
	fcompp
	fstsw   ax
	and     ax, 4300H
	ret
_fcom_r8 endp

end
