public _fcom

.code

_fcom proc
	fld     tbyte ptr [rdx]
	fld     tbyte ptr [rcx]
	fcompp
	fstsw   ax
	and     ax, 4300H
	ret
_fcom endp

end
