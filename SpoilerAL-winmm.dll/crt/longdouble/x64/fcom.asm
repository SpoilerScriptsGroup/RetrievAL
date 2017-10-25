public _fcom

.code

_fcom proc
	fld     tbyte ptr [rdx]
	fld     tbyte ptr [rcx]
	fcompp
	fstsw   ax
	ret
_fcom endp

end
