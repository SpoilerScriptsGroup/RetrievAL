public _ftst

.code

_ftst proc
	fld     tbyte ptr [rcx]
	ftst
	fstsw   ax
	fstp    st(0)
	ret
_ftst endp

end
