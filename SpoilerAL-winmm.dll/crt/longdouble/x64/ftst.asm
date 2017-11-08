public _ftst

.code

_ftst proc
	fld     tbyte ptr [rcx]
	ftst
	fstp    st(0)
	fstsw   ax
	and     ax, 4300H
	ret
_ftst endp

end
