public _fptan

.code

_fptan proc
	fld     tbyte ptr [rdx]
	fptan
	fstp    st(0)
	fstp    tbyte ptr [rcx]
	ret
_fptan endp

end
