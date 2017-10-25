public _fscale

.code

_fscale proc
	fld     tbyte ptr [r8]
	fld     tbyte ptr [rdx]
	fscale
	fstp    st(1)
	fstp    tbyte ptr [rcx]
	ret
_fscale endp

end
