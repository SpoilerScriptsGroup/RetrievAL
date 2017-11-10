public _fxam

.code

_fxam proc
	fld     tbyte ptr [rcx]
	fxam
	fstp    st(0)
	fstsw   ax
	and     ax, 4300H
	ret
_fxam endp

end
