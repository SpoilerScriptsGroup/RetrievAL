public _fchs

.code

_fchs proc
	fld     tbyte ptr [rdx]
	fchs
	fstp    tbyte ptr [rcx]
	ret
_fchs endp

end
