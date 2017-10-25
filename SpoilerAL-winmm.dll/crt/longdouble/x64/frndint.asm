public _frndint

.code

_frndint proc
	fld     tbyte ptr [rdx]
	frndint
	fstp    tbyte ptr [rcx]
	ret
_frndint endp

end
